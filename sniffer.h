#ifndef SNIFFER_H
#define SNIFFER_H

#include <net/ethernet.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <linux/if_arp.h>
#include <assert.h>

#include <iostream>

#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


#include "Connection.h"
#include "parser.h"

class Sniffer{
    Connection conn;
    EthernetParser eth_parser;
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

public:
    Sniffer();
    void run();
};

#endif
