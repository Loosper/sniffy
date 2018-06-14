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


// #include "Connection.h"
// #include "helpers.h"
#include "sniffer.h"

using namespace std;

int main() {
    // Connection conn = Connection();
    // EthernetParser eth_parser = EthernetParser();
    //
    // sql::Driver *driver;
    // sql::Connection *con;
    // sql::Statement *stmt;
    // sql::ResultSet *res;
    // sql::PreparedStatement *pstmt;
    //
    // // conn.bind_to_iface("wlp2s0");
    //
    // try{
    //     driver = get_driver_instance();
    //     con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
    //
    //     con->setSchema("sniffy");
    //
    //     while (1) {
    //         uint8_t *frame = conn.recive();
    //         eth_parser.parse(frame);
    //
    //         delete[] frame;
    //         // break;
    //     }
    // }catch(sql::SQLException &e){
    //     cout << "# ERR: SQLException in " << __FILE__;
    //     cout << "(" << __FUNCTION__ << ") on line "\
    //     << __LINE__ << endl;
    //     cout << "# ERR: " << e.what();
    //     cout << " (MySQL error code: " << e.getErrorCode();
    //     cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    // }

    Sniffer snif = Sniffer();

    snif.run();

    return 0;
}
