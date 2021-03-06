#ifndef PARSER_H
#define PARSER_H

#include <mysql_connection.h>

#include <cstdint>
#include <iostream>
#include <string>

#define IP_TCP    6
#define IP_UDP    17
#define IP_ICMP   1

class Parser {
    protected:
    sql::Connection *con;
    sql::PreparedStatement *mac_query;
    sql::PreparedStatement *ipv4_query;
    sql::PreparedStatement *total_query;

    Parser(sql::Connection *con);
    virtual ~Parser();
    void add_mac(std::string&);
    void add_ipv4(char *);

    public:
    virtual void parse(uint8_t *) = 0;
};

struct arp_body {
    uint8_t src_ha[6];
    uint8_t src_pa[4];
    uint8_t dst_ha[6];
    uint8_t dst_pa[4];
};

class TCPParser: public Parser {
    public:
    TCPParser(sql::Connection *con);
    void parse(uint8_t *segment);
};

class IPParser: public Parser {
    TCPParser *tcp_parser;

    public:
    IPParser(sql::Connection *con);
    ~IPParser();
    void parse(uint8_t *packet);
};


class ARPParser: public Parser {
    public:
    ARPParser(sql::Connection *con);
    void parse(uint8_t *request);
};

class EthernetParser: public Parser {
    IPParser *ip_parser;
    ARPParser *arp_parser;

    public:
    EthernetParser(sql::Connection *con);
    ~EthernetParser();
    void parse(uint8_t *frame);
};


#endif
