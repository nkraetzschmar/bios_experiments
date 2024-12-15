#include "lib.h"

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

uint8 memeq(void *a, void *b, uint16 len)
{
	for (uint16 i = 0; i < len; ++i) if (((uint8 *) a)[i] != ((uint8 *) b)[i]) return 0;
	return 1;
}

void memcpy(void *dst, void *src, uint16 len)
{
	for (uint16 i = 0; i < len; ++i) ((uint8 *) dst)[i] = ((uint8 *) src)[i];
}

void flush()
{
	print_char(0x00);
}

void print_str(const char *str)
{
	for (const char *ptr = str; *ptr; ++ptr) print_char(*ptr);
}

void print_str_len(const char *str, uint16 len)
{
	for (uint16 i = 0; i < len; ++i) print_char(str[i]);
}

static const char *hex_map = "0123456789ABCDEF";

void print_hex(const uint8 *buf, uint16 len)
{
	for (uint16 i = 0; i < len; ++i) {
		uint8 low_bits = buf[i] & 0x0f;
		uint8 high_bits = (buf[i] >> 4) & 0x0f;

		print_char(hex_map[high_bits]);
		print_char(hex_map[low_bits]);
	}
}

char * uint_to_hex(uint32 value, uint16 num_bytes)
{
	static char hex_str[8];
	uint16 num_chars;

	num_chars = num_bytes * 2;
	if (unlikely(num_chars > 8)) num_chars = 8;

	hex_str[num_chars] = 0;

	for (uint16 i = 0; i < num_chars; i++) {
		hex_str[(num_chars - 1) - i] = hex_map[value & 0xF];
		value >>= 4;
	}

	return hex_str;
}

void print_name_value(const char *name, const char *value)
{
	print_str(name);
	print_str("=");
	print_str(value);
	print_str("\r\n");
}
