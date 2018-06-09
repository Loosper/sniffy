#ifndef HELPERS_H
#define HELPERS_H

#include <netinet/ip.h>

#include <cstdint>
#include <string>

std::string format_mac(uint8_t *);
uint16_t calc_ip_checksum(struct ip *);

#endif
