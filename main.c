#include "lib.h"
#include "gpt.h"

uint8 esp_type[0x10] = { 0x28, 0x73, 0x2a, 0xc1, 0x1f, 0xf8, 0xd2, 0x11, 0xba, 0x4b, 0x00, 0xa0, 0xc9, 0x3e, 0xc9, 0x3b };

void init()
{
	static struct gpt_header header;
	static struct gpt_entry entry;

	if (load_gpt(&header) != 0) {
		flush();
		halt();
	}
	if (get_partition(&entry, &header, esp_type) != 0) {
		flush();
		halt();
	}

	flush();
	reset();
}
