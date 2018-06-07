#include <cstdint>
#include <iostream>


void print_mac(uint8_t *mac) {
    printf(
        "%02x:%02x:%02x:%02x:%02x:%02x\n",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]
    );
}
