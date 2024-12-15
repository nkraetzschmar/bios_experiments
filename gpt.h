#include "types.h"

struct gpt_header {
	uint8  signature[0x0008];
	uint16 revision[0x0002];
	uint32 header_size;
	uint32 header_crc32;
	uint32 reserved;
	uint64 header_lba;
	uint64 backup_lba;
	uint64 first_block;
	uint64 last_block;
	uint8  guid[0x10];
	uint64 entries_lba;
	uint32 num_entries;
	uint32 entry_size;
	uint32 entries_crc32;
};

static_assert(sizeof(struct gpt_header) == 0x005c);

struct gpt_entry {
	uint8  type[0x0010];
	uint8  guid[0x0010];
	uint64 start;
	uint64 end;
	uint64 attributes;
	uint16 partition_name[0x0024];
};

static_assert(sizeof(struct gpt_entry) == 0x0080);

void print_guid(uint8 *guid);
int16 load_gpt(struct gpt_header *header);
int16 get_partition(struct gpt_entry *entry, struct gpt_header *header, uint8 *type);
