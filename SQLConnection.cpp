// mysql connector
#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <iostream>

#include "SQLConnection.h"

using namespace std;

SQLConnection::SQLConnection(){
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
    con->setSchema("sniffy");
}

SQLConnection::SQLConnection(string host, string user, string pass, string database){
    driver = get_driver_instance();
    con = driver->connect(host, user, pass);
    con->setSchema(database);
}

SQLConnection::~SQLConnection(){
    delete con;
}

void SQLConnection::run(){
    try {
        execute();
    } catch(sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line "\
        << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;

        // exit(EXIT_FAILURE);
    }
}

void Sniffer::execute() {
    EthernetParser eth_parser = EthernetParser(con);

    while (1) {
        uint8_t *frame = conn.recive();
        eth_parser.parse(frame);

        delete[] frame;
        // break;
    }
}

Selector::Selector(string table, pair<string, string> filter){
    table_ = table;
    filter_ = filter;
}

void Selector::execute() {
    sql::PreparedStatement *pstmt;
    if(table_ == "mac_address"){ 
        pstmt = con->prepareStatement("SELECT * FROM mac_address \
            WHERE ? = ?");
    }else if(table_ == "ipv4_address"){
        pstmt = con->prepareStatement("SELECT * FROM ipv4_address \
            WHERE ? = ?");
    }else if(table_ == "frame"){
        pstmt = con->prepareStatement("SELECT f_id, mac_s.address, \
            mac_d.address, f.total \
            FROM frame f \
            INNER JOIN mac_address mac_s \
                ON f.source = mac_s.id \
            INNER JOIN mac_address mac_d \
                ON f.destination = mac_d.id \
            WHERE ? = ?");
    }else if(table_ == "ipv4_packet"){
        pstmt = con->prepareStatement("SELECT pac.id, ip_s.address, \
            ip_d.address, pac.total_valid, pac.total_invalid \
            FROM ipv4_packet pac \
            INNER JOIN ipv4_address ip_s \
                ON pac.source = ip_s.id \
            INNER JOIN ipv4_address ip_d \
                ON pac.destination = ip_d.id \
            WHERE ? = ?");
    }else if(table_ == "arp_cache"){
        pstmt = con->prepareStatement("SELECT arp.id, mac.address, \
            ip.address, arp.total \
            FROM arp_cache arp \
            INNER JOIN mac_address mac \
                ON arp.mac = mac.id \
            INNER JOIN ipv4_address ip \
                ON arp.ip = ip.id \
            WHERE ? = ?");
    }
    

}
