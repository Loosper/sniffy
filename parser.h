#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include "helpers.h"

using namespace std;

#define IP_TCP    6
#define IP_UDP    17
#define IP_ICMP   1

class Parser {
    virtual void parse(uint8_t *) = 0;
    
    protected:
    sql::PreparedStatement *pstmt;
};

class TCPParser: public Parser {
    public:
    TCPParser(sql::Connection *con){
        pstmt = con->prepareStatement("");
    }

    void parse(uint8_t *segment) {
        struct tcphdr *header = (struct tcphdr *) segment;

        printf("from port: %d\n", ntohs(header->th_sport));
        printf("on port: %d\n", ntohs(header->th_dport));
    }
};

class IPParser: public Parser {
    TCPParser *tcp_parser;

    public:
    IPParser(sql::Connection *con){
        pstmt = con->prepareStatement("");
        tcp_parser = new TCPParser(con);
    }
    
    ~IPParser(){
        delete tcp_parser;
    }

    void parse(uint8_t *packet) {
        struct ip *header = (struct ip *) packet;
        char addr[INET_ADDRSTRLEN];

        int length = header->ip_hl;
        printf("header: %d\n", length);
        printf("total: %d\n", ntohs(header->ip_len));
        printf("next: %d\n", header->ip_p);
        cout << "from: " <<
            inet_ntop(AF_INET, &header->ip_src, addr, INET_ADDRSTRLEN) << endl;
        cout << "to: " <<
            inet_ntop(AF_INET, &header->ip_dst, addr, INET_ADDRSTRLEN) << endl;
        cout << "checksum: " <<
            (header->ip_sum == (calc_ip_checksum(header)) ? "correct" : "wrong") <<
            endl;

        switch (header->ip_p) {
            // in order to compute the checksum, tcp needs parts of the ip packet
            case IP_TCP: tcp_parser->parse(packet + length * 4);
                            printf("TCP");  break;
            case IP_UDP:    printf("UDP");  break;
            case IP_ICMP:   printf("ICMP"); break;
        }

        printf(" segment\n");

    }
};

struct arp_body {
    uint8_t src_ha[6];
    uint8_t src_pa[4];
    uint8_t dst_ha[6];
    uint8_t dst_pa[4];
};

class ARPParser: public Parser {
    public:
    ARPParser(sql::Connection *con){
        pstmt = con->prepareStatement("");
    }

    void parse(uint8_t *request) {
        struct arphdr *header = (struct arphdr *) request;

        // this can't possibly be anyhting else
        assert(ntohs(header->ar_hrd) == ARPHRD_ETHER);
        if (ntohs(header->ar_pro) == ETH_P_IP) {
            printf("iparp\n");
            assert(header->ar_hln == 6);
            assert(header->ar_pln == 4);
        } else {
            return;
        }
        // there's also *_R* of the same
        switch (ntohs(header->ar_op)) {
            case ARPOP_REQUEST: printf("request\n");  break;
            case ARPOP_REPLY: printf("reply\n");  break;
        }

        struct arp_body *body = (struct arp_body *)(request + sizeof(struct arphdr));

        char addr[INET_ADDRSTRLEN];
        cout << "source mac: " << format_mac(body->src_ha) << endl;
        cout << "source ip: " <<
            inet_ntop(AF_INET, body->src_pa, addr, INET_ADDRSTRLEN) << endl;
    }
};

class EthernetParser: public Parser {
    IPParser *ip_parser;
    ARPParser *arp_parser;

    public:
    EthernetParser(sql::Connection *con){
        pstmt = con->prepareStatement("");
        ip_parser = new IPParser(con);
        arp_parser = new ARPParser(con);
    }

    ~EthernetParser(){
        delete ip_parser;
        delete arp_parser;
    }

    void parse(uint8_t *frame) {
        struct ethhdr *header = (struct ethhdr *) frame;

        cout << format_mac(header->h_source) << endl;
        cout << format_mac(header->h_dest) << endl;

        uint8_t *payload = frame + sizeof(struct ethhdr);

        switch (ntohs(header->h_proto)) {
            case ETH_P_IP:
                ip_parser->parse(payload);
                printf("ipv4"); break;
            case ETH_P_IPV6:
                printf("ipv6"); break;
            case ETH_P_ARP:
                arp_parser->parse(payload);
                printf("arp"); break;
            default:
                printf("id: %X", ntohs(header->h_proto));
        }
        printf(" packet\n\n");
    }
};

#endif
