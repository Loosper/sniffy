DROP DATABASE sniffy;
CREATE DATABASE sniffy;

USE sniffy;

CREATE TABLE mac_address (
    id INTEGER NOT NULL AUTO_INCREMENT,
    address BINARY(6) NOT NULL PRIMARY KEY
);

CREATE TABLE ipv4_address (
    id INTEGER NOT NULL AUTO_INCREMENT,
    address BINARY(6) NOT NULL PRIMARY KEY
);

CREATE TABLE frame (
    id INTEGER NOT NULL AUTO_INCREMENT,
    source INTEGER NOT NULL,
    destination INTEGER NOT NULL,
    total INTEGER NOT NULL,

    PRIMARY KEY (source, destination),
    FOREIGN KEY (source) REFERENCES mac_address(id),
    FOREIGN KEY (destination) REFERENCES mac_address(id)
);

CREATE TABLE packet (
    id INTEGER NOT NULL AUTO_INCREMENT,
    source INTEGER NOT NULL,
    destination INTEGER NOT NULL,
    total_valid INTEGER NOT NULL,
    total_invalid INTEGER NOT NULL,

    PRIMARY KEY (source, destination),
-- frame?
    FOREIGN KEY (source) REFERENCES ipv4_address(id),
    FOREIGN KEY (destination) REFERENCES ipv4_address(id)
);

CREATE TABLE arp_cache (
    id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
    mac INTEGER NOT NULL,
    ip INTEGER NOT NULL,

    PRIMARY KEY (mac, ip),
    FOREIGN KEY (mac) REFERENCES mac_address(id),
    FOREIGN KEY (ip) REFERENCES ipv4_address(id)
);
