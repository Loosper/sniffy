#ifndef SNIFFER_H
#define SNIFFER_H

// mysql connector
#include <mysql_connection.h>
#include <cppconn/resultset.h>

// stdlib
#include <iostream>

#include <utility>

#include "Connection.h"
#include "parser.h"

using namespace std;

class SQLConnection {
protected:
    sql::Driver *driver;
    sql::Connection *con;

    SQLConnection();
    SQLConnection(std::string, std::string, std::string, std::string);
    ~SQLConnection();

public:
    void run();
    virtual void execute() = 0;
};

class Sniffer: public SQLConnection {
    Connection conn;
    void execute();
};

class Selector: public SQLConnection {
    string table_;
    pair<string, string> filter_;

    void execute();

public:
    Selector(string, pair<string, string>);
};


#endif
