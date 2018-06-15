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

Parser::~Parser() {
    delete mac_query;
    delete ipv4_query;
    delete total_query;
}

Parser::Parser(sql::Connection *con) {
    this->con = con;
    mac_query = con->prepareStatement(
        "INSERT IGNORE INTO mac_address (address) VALUES (?)"
    );

    ipv4_query = con->prepareStatement(
        "INSERT IGNORE INTO ipv4_address (address) VALUES (?)"
    );
}

void Parser::add_mac(string& mac) {
    mac_query->setString(1, mac);
    mac_query->executeUpdate();
}

void Parser::add_ipv4(char *ip) {
    ipv4_query->setString(1, ip);
    ipv4_query->executeUpdate();
}

TCPParser::TCPParser(sql::Connection *con): Parser(con) {
}

void TCPParser::parse(uint8_t *segment) {
    struct tcphdr *header = (struct tcphdr *) segment;

    printf("from port: %d\n", ntohs(header->th_sport));
    printf("on port: %d\n", ntohs(header->th_dport));
}

IPParser::IPParser(sql::Connection *con): Parser(con) {
    tcp_parser = new TCPParser(con);

    total_query = con->prepareStatement(
        "INSERT INTO ipv4_packet (source, destination, total_valid, total_invalid)\
        VALUES (\
            (SELECT id FROM ipv4_address WHERE address = ?),\
            (SELECT id FROM ipv4_address WHERE address = ?),\
            ?, ?\
        )\
        ON DUPLICATE KEY UPDATE\
        total_valid = total_valid + ?, total_invalid = total_invalid + ?;"
    );
}

IPParser::~IPParser() {
    delete tcp_parser;
}

void IPParser::parse(uint8_t *packet) {
    struct ip *header = (struct ip *) packet;
    char src_addr[INET_ADDRSTRLEN];
    char dest_addr[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &header->ip_src, src_addr, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &header->ip_dst, dest_addr, INET_ADDRSTRLEN);

    int length = header->ip_hl;
    int valid = header->ip_sum == calc_ip_checksum(header);

    printf("header: %d\n", length);
    printf("total: %d\n", ntohs(header->ip_len));
    printf("next: %d\n", header->ip_p);

    cout << "from: " << src_addr << endl;
    cout << "to: " << dest_addr << endl;
    cout << "checksum: " << ((valid) ? "correct" : "wrong") << endl;

    switch (header->ip_p) {
        // in order to compute the checksum, tcp needs parts of the ip packet
        case IP_TCP: tcp_parser->parse(packet + length * 4);
                        printf("TCP");  break;
        case IP_UDP:    printf("UDP");  break;
        case IP_ICMP:   printf("ICMP"); break;
    }

    printf(" segment\n");

    add_ipv4(src_addr);
    add_ipv4(dest_addr);

    // cout << "validity: " << valid << "  "<< !valid << endl;

    total_query->setString(1, src_addr);
    total_query->setString(2, dest_addr);
    total_query->setInt(3, valid);
    total_query->setInt(4, !valid);
    // i dont know sql; this there could be a way without this
    total_query->setInt(5, valid);
    // BUG: this is wrong
    total_query->setInt(6, !valid);
    total_query->executeUpdate();
}

ARPParser::ARPParser(sql::Connection *con): Parser(con) {
    // TODO: add type
    total_query = con->prepareStatement(
        "INSERT INTO arp_cache (mac, ip, total, type)\
        VALUES (\
            (SELECT id FROM mac_address WHERE address = ?),\
            (SELECT id FROM ipv4_address WHERE address =  ?),\
            ?, ?\
        ) ON DUPLICATE KEY UPDATE total = total + 1;"
    );
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
    int arp_type = ntohs(header->ar_op);;
    // there's also *_R* of the same
    switch (arp_type) {
        case ARPOP_REQUEST: printf("request\n");  break;
        case ARPOP_REPLY: printf("reply\n");  break;
    }

    struct arp_body *body = (struct arp_body *)(request + sizeof(struct arphdr));

    char src_ip[INET_ADDRSTRLEN] = {0};
    string src_mac = format_mac(body->src_ha);
    if (inet_ntop(AF_INET, body->src_pa, src_ip, INET_ADDRSTRLEN) == NULL) {
        perror("bad ip?");
    }

    cout << "source mac: " << src_mac << endl;
    cout << "source ip: " << src_ip << endl;

    add_mac(src_mac);
    add_ipv4(src_ip);

    total_query->setString(1, src_mac);
    total_query->setString(2, src_ip);
    total_query->setInt(3, 1);
    total_query->setInt(4, arp_type);
    total_query->executeUpdate();
}

EthernetParser::EthernetParser(sql::Connection *con): Parser(con) {
    total_query = con->prepareStatement(
        "INSERT INTO frame (source, destination, total)\
        VALUES (\
            (SELECT id FROM mac_address WHERE address = ?),\
            (SELECT id FROM mac_address WHERE address =  ?),\
            ?\
        ) ON DUPLICATE KEY UPDATE total = total + 1;"
    );

    ip_parser = new IPParser(con);
    arp_parser = new ARPParser(con);
}

EthernetParser::~EthernetParser() {
    delete ip_parser;
    delete arp_parser;
}

void EthernetParser::parse(uint8_t *frame) {
    struct ethhdr *header = (struct ethhdr *) frame;

    if (header->h_source[0] == 0)
        return;

    string src = format_mac(header->h_source);
    string dest = format_mac(header->h_dest);

    cout << src << endl;
    cout << dest << endl;

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

    add_mac(src);
    add_mac(dest);

    total_query->setString(1, src);
    total_query->setString(2, dest);
    total_query->setInt(3, 1);
    total_query->executeUpdate();
}
