#ifndef BUILDER_H
#define BUILDER_H

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "SQLConnection.h"

#define HOST "localhost"
#define USER "root"
#define PASS "root"
#define DB "sniffy"

using namespace std;

class Builder: public SQLConnection {
public:
    Builder() : SQLConnection(){}
    Builder(string host, string user, string pass, string database) : SQLConnection(host, user, pass, database){}

    void execute() {
        // string url(HOST);
        // const string user(USER);
        // const string pass(PASS);
        // const string database(DB);
    }
};

int main() {
    Builder builder = Builder();

    builder.run();
}

#endif
