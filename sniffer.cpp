#include "sniffer.h"

SQLConnection::SQLConnection(){
    conn = Connection();
}

void SQLConnection::run(){
    try{
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "root");

        con->setSchema("sniffy");

        execute();

    }catch(sql::SQLException &e){
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line "\
        << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
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
