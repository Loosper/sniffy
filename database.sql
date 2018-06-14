DROP DATABASE sniffy;
CREATE DATABASE sniffy;

USE sniffy;

CREATE TABLE mac_address (
    id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
    address BINARY(6) NOT NULL
);

CREATE TABLE ipv4_address (
    id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
    address BINARY(6) NOT NULL
);

CREATE TABLE frame (
    id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
    source INTEGER NOT NULL,
    destination INTEGER NOT NULL,
    total INTEGER NOT NULL,

    FOREIGN KEY (source) REFERENCES mac_address(id),
    FOREIGN KEY (destination) REFERENCES mac_address(id)
);

CREATE TABLE packet (
    id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
    source INTEGER NOT NULL,
    destination INTEGER NOT NULL,
    total_valid INTEGER NOT NULL,
    total_invalid INTEGER NOT NULL,

-- frame?
    FOREIGN KEY (source) REFERENCES ipv4_address(id),
    FOREIGN KEY (destination) REFERENCES ipv4_address(id)
);

CREATE TABLE arp_cache (
    id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
    mac INTEGER NOT NULL,
    ip INTEGER NOT NULL,

    FOREIGN KEY (mac) REFERENCES mac_address(id),
    FOREIGN KEY (ip) REFERENCES ipv4_address(id)
);

CREATE UNIQUE INDEX mac_duplicate ON mac_address(address);
CREATE UNIQUE INDEX ipv4_duplicate ON ipv4_address(address);

CREATE UNIQUE INDEX frame_duplicate ON frame(source, destination);
CREATE UNIQUE INDEX ipv4_duplicate ON ipv4(source, destination);
CREATE UNIQUE INDEX arp_duplicate ON arp_cache(ip, mac);
