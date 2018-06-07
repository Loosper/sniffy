#include <cstdint>
#include <iostream>
#include <sstream>


 std::string parse_mac(uint8_t *mac) {
     std::stringstream stream;

     stream << std::hex << mac[0]
     << ":" << std::hex << mac[1]
     << ":" << std::hex << mac[2]
     << ":" << std::hex << mac[3]
     << ":" << std::hex << mac[4]
     << ":" << std::hex << mac[5];


    return stream.str();

    /*
    printf(
        "%02x:%02x:%02x:%02x:%02x:%02x\n",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]
    );
    */
}
