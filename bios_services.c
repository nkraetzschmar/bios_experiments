#include "bios_services.h"

void halt()
{
	while (1) __asm__ __volatile__ ("hlt");
}

void reset()
{
	__asm__ __volatile__ ("ljmp $0xffff, $0x0000");
}

void print_char(char c)
{
	__asm__ __volatile__ ("int $0x10" : : "ax" (0x0e00 | c) : );
}

int16 disk_access(uint16 op, uint8 *buffer, uint16 sectors, uint32 lba)
{
	static struct disk_address_packet {
		uint8 size;
		uint8 reserved;
		uint16 sectors;
		uint32 buffer;
		uint64 lba;
	} disk_address_packet;

	uint8 result;
	uint8 error;

	static_assert(sizeof(disk_address_packet) == 0x10);

	disk_address_packet = (struct disk_address_packet) {
		.size = 0x10,
		.reserved = 0x00,
		.sectors = sectors,
		.buffer = (uint32) buffer,
		.lba = { lba, 0x00000000 }
	};

	__asm__ __volatile__ ("mov %[dap], %%si\nint $0x13" : "=cf" (result), "=ah" (error) : "ax" (op), "dx" (0x0080), [dap] "i" (&disk_address_packet) : );

	return result ? -error : 0;
}

int16 disk_read(uint8 *buffer, uint16 sectors, uint32 lba)
{
	return disk_access(0x4200, buffer, sectors, lba);
}

int16 disk_write(uint8 *buffer, uint16 sectors, uint32 lba)
{
	return disk_access(0x4300, buffer, sectors, lba);
}
