// mysql connector
#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <iostream>

#include <algorithm>
#include <string>

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

Selector::Selector(string table){
    table_ = table;
}

void Selector::execute() {
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;

    string mac_address[] = {"ID", "ADDRESS", "NULL"};
    string ipv4_address[] = {"ID", "ADDRESS", "NULL"};
    string frame[] = {"ID", "SOURCE", "DESTINATION", "TOTAL", "NULL"};
    string ipv4_packet[] = {"ID", "SOURCE", "DESTINATION",
                "TOTAL_VALID", "TOTAL_INVALID", "NULL"};
    string arp_cache[] = {"ID", "MAC", "IP", "TYPE","TOTAL", "NULL"};

    string *fields;

    if(table_ == "mac_address"){
        pstmt = con->prepareStatement("SELECT id AS \'ID\', address AS \'ADDRESS\' \
            FROM mac_address");
        fields = mac_address;
    }else if(table_ == "ipv4_address"){
        pstmt = con->prepareStatement("SELECT id AS \'ID\', address AS \'ADDRESS\' \
            FROM ipv4_address");
        fields = ipv4_address;
    }else if(table_ == "frame"){
        pstmt = con->prepareStatement("SELECT f.id AS \'ID\', \
            mac_s.address AS \'SOURCE\', \
            mac_d.address AS \'DESTINATION\', f.total AS \'TOTAL\'\
            FROM frame f \
            INNER JOIN mac_address mac_s \
                ON f.source = mac_s.id \
            INNER JOIN mac_address mac_d \
                ON f.destination = mac_d.id");
        fields = frame;
    }else if(table_ == "ipv4_packet"){
        pstmt = con->prepareStatement("SELECT pac.id AS \'ID\', \
            ip_s.address AS \'SOURCE\', ip_d.address AS \'DESTINATION\', \
            pac.total_valid AS \'TOTAL_VALID\', \
            pac.total_invalid AS \'TOTAL_INVALID\' \
            FROM ipv4_packet pac \
            INNER JOIN ipv4_address ip_s \
                ON pac.source = ip_s.id \
            INNER JOIN ipv4_address ip_d \
                ON pac.destination = ip_d.id");
        fields = ipv4_packet;
    }else if(table_ == "arp_cache"){
        pstmt = con->prepareStatement("SELECT arp.id AS \'ID\', \
            mac.address AS \'MAC\', \
            ip.address AS \'IP\', type.name AS \'TYPE\',\
            arp.total AS \'TOTAL\' \
            FROM arp_cache arp \
            INNER JOIN mac_address mac \
                ON arp.mac = mac.id \
            INNER JOIN ipv4_address ip \
                ON arp.ip = ip.id \
            INNER JOIN arp_type type \
                ON arp.type = type.id");
        fields = arp_cache;
    }

    res = pstmt->executeQuery();

    std::transform(table_.begin(), table_.end(),table_.begin(), ::toupper);

    cout << table_ << endl;

    cout << "|  ";
    for(int i = 0 ; fields[i] != "NULL" ; ++ i){
        cout << fields[i] << "  |  ";
    }

    cout << endl;

    while(res->next()){
        for(int i = 0 ; fields[i] != "NULL" ; ++ i){
            if(i == 0)
                cout << res->getInt("ID") << " ";
            else
                cout << res->getString(fields[i]) << " ";
        }
        cout << endl;
    }
}
