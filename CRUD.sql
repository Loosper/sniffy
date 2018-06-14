INSERT INTO mac_address (address)
VALUES (?);

INSERT INTO ipv4_address (address)
VALUES (?);

INSERT INTO frame (source, destination, total)
VALUES (?, ?, ?)
ON DUPLICATE KEY UPDATE total = total + 1;

-- the last two should be 0 or 1 depending on what happened
INSERT INTO ipv4_packet (source, destination, total_valid, total_invalid)
VALUES (?, ?, ?, ?)
ON DUPLICATE KEY UPDATE
total_valid = total_valid + ?, total_invalid = total_invalid + ?;

INSERT INTO arp_cache (mac, ip, total)
VALUES (?, ?, ?)
ON DUPLICATE KEY UPDATE total = total + 1;
