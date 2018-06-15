#ifndef BUILDER_H
#define BUILDER_H

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <fstream>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "SQLConnection.h"

#define HOST "127.0.0.1:3306"
#define USER "root"
#define PASS "root"
#define DB "sniffy"

using namespace std;

class Builder: public SQLConnection {
public:
    Builder() : SQLConnection(){}
    Builder(string host, string user, string pass, string database) : SQLConnection(host, user, pass, database){}

    void execute() {

        // ifstream t("database.sql");
        // stringstream buffer;
        // buffer << t.rdbuf();

        sql::Statement *stmt;

        stmt = con->createStatement();

        //Everything must drop in this order
        stmt->execute("DROP TABLE IF EXISTS arp_cache");
        stmt->execute("DROP TABLE IF EXISTS ipv4_packet");
        stmt->execute("DROP TABLE IF EXISTS frame");
        stmt->execute("DROP TABLE IF EXISTS ipv4_address");
        stmt->execute("DROP TABLE IF EXISTS mac_address");

        stmt->execute("CREATE TABLE mac_address (\
            id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,\
            address BINARY(6) NOT NULL\
        )");

        stmt->execute("CREATE TABLE ipv4_address (\
            id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,\
            address BINARY(4) NOT NULL\
        )");

        stmt->execute("CREATE TABLE frame (\
            id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,\
            source INTEGER NOT NULL,\
            destination INTEGER NOT NULL,\
            total INTEGER NOT NULL,\
            FOREIGN KEY (source) REFERENCES mac_address(id),\
            FOREIGN KEY (destination) REFERENCES mac_address(id)\
        );");

        stmt->execute("CREATE TABLE ipv4_packet (\
            id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,\
            source INTEGER NOT NULL,\
            destination INTEGER NOT NULL,\
            total_valid INTEGER NOT NULL,\
            total_invalid INTEGER NOT NULL,\
            FOREIGN KEY (source) REFERENCES ipv4_address(id),\
            FOREIGN KEY (destination) REFERENCES ipv4_address(id)\
        );");

        stmt->execute("CREATE TABLE arp_cache (\
            id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,\
            mac INTEGER NOT NULL,\
            total INTEGER NOT NULL,\
            ip INTEGER NOT NULL,\
            FOREIGN KEY (mac) REFERENCES mac_address(id),\
            FOREIGN KEY (ip) REFERENCES ipv4_address(id)\
        );");

        stmt->execute("CREATE UNIQUE INDEX mac_duplicate ON mac_address(address)");
        stmt->execute("CREATE UNIQUE INDEX ipv4_duplicate ON ipv4_address(address);");
        stmt->execute("CREATE UNIQUE INDEX frame_duplicate ON frame(source, destination)");
        stmt->execute("CREATE UNIQUE INDEX ipv4_duplicate ON ipv4_packet(source, destination)");
        stmt->execute("CREATE UNIQUE INDEX arp_duplicate ON arp_cache(ip, mac)");

        delete stmt;

        cout << "Database Schema built successfully" << endl;
    }
};

int main() {
    Builder builder = Builder();

    builder.run();
}

#endif
