DROP DATABASE IF EXISTS sniffy;
CREATE DATABASE sniffy;

USE sniffy;

CREATE TABLE mac_address (
    id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
    address CHAR(17) NOT NULL
);

CREATE TABLE ipv4_address (
    id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
    address VARCHAR(16) NOT NULL
);

CREATE TABLE arp_type (
    id INTEGER NOT NULL PRIMARY KEY,
    name VARCHAR(50)
);

CREATE TABLE frame (
    id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,
    source INTEGER NOT NULL,
    destination INTEGER NOT NULL,
    total INTEGER NOT NULL,

    FOREIGN KEY (source) REFERENCES mac_address(id),
    FOREIGN KEY (destination) REFERENCES mac_address(id)
);

CREATE TABLE ipv4_packet (
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
    type INTEGER NOT NULL,
    total INTEGER NOT NULL,

    FOREIGN KEY (mac) REFERENCES mac_address(id),
    FOREIGN KEY (ip) REFERENCES ipv4_address(id),
    FOREIGN KEY (type) REFERENCES arp_type(id)
);

CREATE UNIQUE INDEX mac_duplicate ON mac_address(address);
CREATE UNIQUE INDEX ipv4_duplicate ON ipv4_address(address);
CREATE UNIQUE INDEX arp_type_duplicate ON arp_type(name);

CREATE UNIQUE INDEX frame_duplicate ON frame(source, destination);
CREATE UNIQUE INDEX ipv4_packet_duplicate ON ipv4_packet(source, destination);
CREATE UNIQUE INDEX arp_duplicate ON arp_cache(ip, mac, type);

INSERT INTO arp_type (id, name) VALUES (1, 'REQUEST'), (2, 'REPLY');
