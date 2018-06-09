#include <net/ethernet.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include <iostream>

#include "Connection.h"
#include "helpers.h"

using namespace std;

#define IP_TCP    6
#define IP_UDP    17
#define IP_ICMP   1

class Parser {
    virtual void parse(uint8_t *) = 0;
};

class TCPParser: public Parser {
    public:
    void parse(uint8_t *segment) {
        struct tcphdr *header = (struct tcphdr *) segment;

        printf("from port: %d\n", ntohs(header->th_sport));
        printf("on port: %d\n", ntohs(header->th_dport));
    }
};

class IPParser: public Parser {
    TCPParser tcp_parser = TCPParser();

    public:
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

        switch (header->ip_p) {
            case IP_TCP: tcp_parser.parse(packet + length * 4);
                            printf("TCP");  break;
            case IP_UDP:    printf("UDP");  break;
            case IP_ICMP:   printf("ICMP"); break;
        }

        printf(" segment\n");
    }
};

class EthernetParser: public Parser {
    IPParser ip_parser = IPParser();

    public:
    void parse(uint8_t *frame) {
        struct ethhdr *header = (struct ethhdr *) frame;

        cout << format_mac(header->h_source) << endl;
        cout << format_mac(header->h_dest) << endl;

        switch (ntohs(header->h_proto)) {
            case ETH_P_IP:
                ip_parser.parse(frame + sizeof(struct ethhdr));
                printf("ipv4"); break;
            case ETH_P_IPV6:
                printf("ipv6"); break;
            case ETH_P_ARP:
                printf("arp"); break;
            default:
                printf("id: %X", ntohs(header->h_proto));
        }
        printf(" packet\n\n");
    }
};


int main() {
    Connection conn = Connection();
    EthernetParser eth_parser = EthernetParser();

    // conn.bind_to_iface("wlp2s0");

    while (1) {
        uint8_t *frame = conn.recive();
        eth_parser.parse(frame);

        delete[] frame;
        // break;
    }

    return 0;
}
