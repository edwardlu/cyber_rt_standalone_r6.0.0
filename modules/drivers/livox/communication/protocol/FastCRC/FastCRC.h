#ifndef FASTCRC_FASTCRC_H_
#define FASTCRC_FASTCRC_H_

#include <stdint.h>


// ================= 16-BIT CRC ===================
class FastCRC16 {
public:
	FastCRC16(uint16_t seed);

	// change function name from mcrf4xx_upd to mcrf4xx
	uint16_t mcrf4xx_calc(const uint8_t *data,const uint16_t datalen); // Equivalent to _crc_ccitt_update() in crc16.h from avr_libc

private:
	uint16_t seed_;
};

// ================= 32-BIT CRC ===================
class FastCRC32 {
public:
	FastCRC32(uint32_t seed);

	// change function name from crc32_upd to crc32
	uint32_t crc32_calc(const uint8_t *data, uint16_t len);	// Call for subsequent calculations with previous seed

private:
	uint32_t seed_;
};

#endif // FASTCRC_FASTCRC_H_

