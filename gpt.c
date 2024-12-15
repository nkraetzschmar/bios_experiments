#include "gpt.h"
#include "lib.h"

static uint8 buf[0x200];

void print_guid(uint8 *guid)
{
	print_str(uint_to_hex(*((uint32 *) guid), 0x04));
	print_str("-");
	print_str(uint_to_hex(*((uint16 *) (guid + 0x04)), 0x02));
	print_str("-");
	print_str(uint_to_hex(*((uint16 *) (guid + 0x06)), 0x02));
	print_str("-");
	print_hex(guid + 0x08, 0x02);
	print_str("-");
	print_hex(guid + 0x0a, 0x06);
}

uint8 lba_in_range(uint64 lba) {
	if (lba.high != 0 || lba.low > 0x7fffffff) {
		print_str("LBA exceeds supported range\r\n");
		return 0;
	}

	return 1;
}

int16 load_gpt(struct gpt_header *dst)
{
	struct gpt_header *header;

	disk_read(buf, 1, 1);
	header = (struct gpt_header *) buf;

	if (!memeq(header->signature, "EFI PART", sizeof(header->signature))) {
		print_str("not a valid GPT header\r\n");
		return -1;
	}

	if (!lba_in_range(header->entries_lba)) return -1;

	print_str("loaded GPT header GUID=");
	print_guid(header->guid);
	print_str("\r\n");

	memcpy(dst, header, sizeof(*header));
	return 0;
}

int16 get_partition(struct gpt_entry *dst, struct gpt_header *header, uint8 *type)
{
	uint32 num_blocks;
	uint32 lba;
	struct gpt_entry *entries;
	
	num_blocks = header->num_entries / 4;
	lba = header->entries_lba.low;

	for (uint32 i = 0; i < num_blocks; ++i) {
		disk_read(buf, 1, lba++);
		entries = (struct gpt_entry *) buf;

		for (uint16 i = 0; i < 4; ++i) {
			if (memeq(&entries[i].type, type, sizeof(entries[i].type))) {
				if (!lba_in_range(entries[i].start) || !lba_in_range(entries[i].end)) return -1;

				print_str("found partition GUID=");
				print_guid(entries[i].guid);
				print_str("\r\n");

				memcpy(dst, &entries[i], sizeof(entries[i]));
				return 0;
			}
		}
	}
	
	return -1;
}
