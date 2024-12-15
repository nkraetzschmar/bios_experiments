MAKEFLAGS += --no-builtin-rules
.SILENT:
.PHONY: clean test debug

disk: mbr.bin bootloader.bin
	echo 'writing $^ -> $@'
	truncate -s 0 '$@'
	truncate -s 1G '$@'
	printf 'label: gpt\nlabel-id: 01234567-abcd-0123-abcd-0123456789ab\ntype=uefi, uuid=aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee, name=EFI\n' | sfdisk '$@'
	dd if=mbr.bin of='$@' bs=446 count=1 conv=notrunc 2> /dev/null
	dd if=bootloader.bin of='$@' bs=512 count=16 seek=34 conv=notrunc 2> /dev/null

bootloader.elf: main.o gpt.o lib.o bios_services.o

dependencies.make: *.c
	x86_64-linux-gnu-gcc -nostdinc -MM $^ > '$@'

include dependencies.make

%.o:
	echo 'compiling $< -> $@'
	x86_64-linux-gnu-gcc -std=c23 -Os -g -Wall -Wextra -Wdeclaration-after-statement -Werror -m16 -march=i386 -nostdinc -ffreestanding -fno-pic -fno-stack-protector -ffunction-sections -fdata-sections -fpack-struct -c '$<'
	x86_64-linux-gnu-objdump -m i8086 -M intel -h -d '$@'

%.elf: %.ld
	echo 'linking $^ -> $@'
	x86_64-linux-gnu-ld -m elf_i386 -o '$@' -T $^

%.bin: %.elf
	echo 'writing $< -> $@'
	x86_64-linux-gnu-objcopy -O binary '$<' '$@'
	hexdump -C '$@'

%.bin: %.asm
	echo 'building $< -> $@'
	nasm -f bin -o '$@' '$<'
	hexdump -vC '$@'

clean:
	rm -f disk *.bin *.elf *.o

test: disk
	echo 'running $< in qemu'
	./run.sh -drive file='$<',format=raw

debug: disk | bootloader.elf
	echo 'running $< in qemu in debug mode'
	./debug.sh '$|' -drive file='$<',format=raw
