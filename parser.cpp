// socket libraries
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <linux/if_arp.h>
#include <assert.h>

// mysql connector
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

// stdlib
#include <iostream>

#include "helpers.h"
#include "parser.h"

using namespace std;


TCPParser::TCPParser(sql::Connection *con){
    // pstmt = con->prepareStatement("");
}

void TCPParser::parse(uint8_t *segment) {
    struct tcphdr *header = (struct tcphdr *) segment;

    printf("from port: %d\n", ntohs(header->th_sport));
    printf("on port: %d\n", ntohs(header->th_dport));
}

IPParser::IPParser(sql::Connection *con){
    // pstmt = con->prepareStatement("");
    tcp_parser = new TCPParser(con);
}

IPParser::~IPParser(){
    delete tcp_parser;
}

void IPParser::parse(uint8_t *packet) {
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

ARPParser::ARPParser(sql::Connection *con){
    // pstmt = con->prepareStatement("");
}

void ARPParser::parse(uint8_t *request) {
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

EthernetParser::EthernetParser(sql::Connection *con){
    // pstmt = con->prepareStatement(
    //     "INSERT INTO mac_address (address) VALUES (?)"
    // );
    ip_parser = new IPParser(con);
    arp_parser = new ARPParser(con);
}

EthernetParser::~EthernetParser(){
    delete ip_parser;
    delete arp_parser;
}

void EthernetParser::parse(uint8_t *frame) {
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
