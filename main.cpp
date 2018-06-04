#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>

#include <iostream>
#include <cstdint>


#include <cstdlib>

// ???
using namespace std;


class Connection {
    private:
    int conn;

    public:
    Connection() {
        conn = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

        if (conn == -1) {
            perror("socket failed. Check you have given superuser privileges");
            exit(0);
        }
    }

    void recive() {
        // uint8_t *buffer = new uint8_t[ETHER_MAX_LEN]();
        uint8_t *buffer = (uint8_t *) malloc(ETHER_MAX_LEN);
        // TODO: see what is in here and discard if pointless
        // most likely pointless
        struct sockaddr addr;
        socklen_t addr_len = sizeof(addr);

        // that 0 means no flags
        // consider a packet that is too big
        if (recvfrom(conn, buffer, ETHER_MAX_LEN, 0, &addr, &addr_len) == -1) {
            perror("worng");
            // for now; we could print a message
            exit(0);
        }

        struct ethhdr *header = (struct ethhdr *) buffer;
        printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
            header->h_dest[0],
            header->h_dest[1],
            header->h_dest[2],
            header->h_dest[3],
            header->h_dest[4],
            header->h_dest[5]
        );

        printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
            header->h_source[0],
            header->h_source[1],
            header->h_source[2],
            header->h_source[3],
            header->h_source[4],
            header->h_source[5]
        );
        printf("%x\n", ntohs(header->h_proto));
    }
};


int main() {
    Connection conn = Connection();
    conn.recive();

    return 0;
}
