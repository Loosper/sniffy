#include <netinet/ip.h>
#include <cstdint>
#include <iostream>
#include <string>

#include "helpers.h"

std::string format_mac(uint8_t *mac) {
    char smac[18] = {'\0'};

    sprintf(
        smac,
        "%02X:%02X:%02X:%02X:%02X:%02X",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]
    );

    return std::string(smac);
}

uint16_t calc_ip_checksum(struct ip *header) {
    uint32_t checksum = 0;
    int size = header->ip_hl * 2;
    uint16_t *plain = (uint16_t *) header;

    for (int i = 0; i < size; i++) {
        checksum += plain[i];
    }

    // this is not used in the calcualtion
    checksum -= header->ip_sum;

    uint32_t carry;
    while ((carry = checksum & 0xFF0000) != 0) {
        checksum = (checksum & 0x00FFFF) + (carry >> 16);
    }

    return ~ (uint16_t) checksum;
}
