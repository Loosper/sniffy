#include <net/ethernet.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

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

class IPParser: public Parser {
    public:
    void parse(uint8_t *packet) {
        struct ip *header = (struct ip *) packet;

        // int length = header->ip_hl;
        printf("header: %d\n", header->ip_hl);
        printf("total: %d\n", ntohs(header->ip_len));
        printf("next: %d\n", header->ip_p);
        switch (header->ip_p) {
            case IP_TCP:    printf("TCP");   break;
            case IP_UDP:    printf("UDP");   break;
            case IP_ICMP:   printf("ICMP");  break;
        }

        printf(" segment\n");
    }
};

class EthernetParser: public Parser {
    public:
    void parse(uint8_t *frame) {
        struct ethhdr *header = (struct ethhdr *) frame;
        IPParser ip_parser = IPParser();

        cout << print_mac(header->h_source) << endl;
        cout << print_mac(header->h_dest) << endl;

        switch (ntohs(header->h_proto)) {
            case ETH_P_IP:
                ip_parser.parse(frame + sizeof(struct ethhdr)); printf("ipv4"); break;
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
