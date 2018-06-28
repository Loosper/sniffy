#ifndef CONNECTION_H
#define CONNECTION_H

#include <cstdint>

class Connection {
    private:
    int conn;
    // Parser eth_parser;

    public:
    Connection();
    ~Connection();
    uint8_t *recive();
    void bind_to_iface(char *iface);
    void set_promiscuous(char *);
};

#endif
