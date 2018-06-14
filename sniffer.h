#ifndef SNIFFER_H
#define SNIFFER_H

#include <iostream>

#include <net/ethernet.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <linux/if_arp.h>
#include <assert.h>


#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


#include "Connection.h"
#include "parser.h"

class SQLConnection {
    public:
    Connection conn;
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    SQLConnection();
    void run();
    virtual void execute() = 0;
};

class Sniffer: public SQLConnection {
    void execute();
};

#endif
