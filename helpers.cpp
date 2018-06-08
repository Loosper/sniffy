#include <cstdint>
#include <iostream>
#include <string>


std::string format_mac(uint8_t *mac) {
    char smac[18] = {'\0'};

    sprintf(
        smac,
        "%02X:%02X:%02X:%02X:%02X:%02X",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]
    );

    return std::string(smac);
}
