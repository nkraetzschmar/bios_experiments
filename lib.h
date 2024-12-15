#include "types.h"
#include "bios_services.h"

uint8 memeq(void *a, void *b, uint16 len);
void memcpy(void *dst, void *src, uint16 len);

void flush();
void print_str(const char *str);
void print_hex(const uint8 *buf, uint16 len);

char * uint_to_hex(uint32 value, uint16 num_bytes);
void print_name_value(const char *name, const char *value);

#define print_var(X) print_name_value(#X, _Generic((X), \
	uint8: uint_to_hex(X, 1), \
	uint16: uint_to_hex(X, 2), \
	uint32: uint_to_hex(X, 4) \
))
