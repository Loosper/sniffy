// mysql connector
#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include "SQLConnection.h"

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
