#ifndef SNIFFER_H
#define SNIFFER_H

// mysql connector
#include <mysql_connection.h>
#include <cppconn/resultset.h>

// stdlib
#include <iostream>

#include "Connection.h"
#include "parser.h"

class SQLConnection {
protected:
    sql::Driver *driver;
    sql::Connection *con;

    SQLConnection();
    SQLConnection(std::string, std::string, std::string, std::string);

public:
    void run();
    virtual void execute() = 0;
};

class Sniffer: public SQLConnection {
    Connection conn;
    void execute();
};

#endif
