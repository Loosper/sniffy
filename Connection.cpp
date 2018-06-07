#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <unistd.h>

#include <cstdint>
#include <cstring>

#include <iostream>

#include "Connection.h"

Connection::Connection() {
    conn = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (conn == -1) {
        perror("socket failed. Check you have given superuser privileges");
        exit(0);
    }
}

Connection::~Connection() {
    close(conn);
}

uint8_t *Connection::recive() {
    uint8_t *buffer = new uint8_t[ETHER_MAX_LEN]();

    // that 0 means no flags
    // consider a packet that is too big
    ssize_t bytes = recv(conn, buffer, ETHER_MAX_LEN, 0);
    if (bytes == -1) {
        perror("worng");
        // for now; we could print a message
        exit(0);
    }

    printf("recived %ld bytes\n", bytes);

    return buffer;
}

void Connection::bind_to_iface(char *iface) {
    size_t len = strnlen(iface, IFNAMSIZ);

    if (len == IFNAMSIZ) {
        std::cout << "Interface name too long" << std::endl;
        exit(0);
    }

    // bind is also valid
    if (setsockopt(conn, SOL_SOCKET, SO_BINDTODEVICE, iface, len) == -1) {
        perror("Something went wrong");
        exit(0);
    }
}

// void Connection::set_promiscuous() {
//     struct ifreq ifr;
//
//     ifr.ifr_flags |= IFF_PROMISC;
//     if (ioctl(conn, SIOCSIFFLAGS, &ifr) == -1) {
//         perror("Ioctl failed");
//         exit(0);
//     }
// }
