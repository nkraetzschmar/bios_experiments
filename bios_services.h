#include "types.h"

void halt();
void reset();
void print_char(char c);
int16 disk_read(uint8 *buffer, uint16 sectors, uint32 lba);
int16 disk_write(uint8 *buffer, uint16 sectors, uint32 lba);
