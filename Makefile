AS=i686-elf-as
CC=i686-elf-gcc
LD=i686-elf-gcc
CFLAGS=-c -Isrc/include -std=gnu99 -ffreestanding -Wall -Wextra -Werror -Wpedantic
LDFLAGS=-T linker.ld -ffreestanding -nostdlib -lgcc

CRTBEGIN=$(shell $(CC) $(CFLAGS) --print-file-name=crtbegin.o)
CRTEND=$(shell $(CC) $(CFLAGS) --print-file-name=crtend.o)

OBJECTS=src/kernel.o src/vga.o src/string.o src/printf.o src/gdt.o src/segment_reload.o src/multiboot.o
HEADERS=$(wildcard src/*.h)

LINK_LIST=src/boot.o src/crti.o $(CRTBEGIN) $(OBJECTS) $(CRTEND) src/crtn.o

.PHONY: all
all: kenshi.iso

.PHONY: run
run: all
	qemu-system-i386 -cdrom kenshi.iso

kenshi.iso: kenshi.bin
	grub-file --is-x86-multiboot kenshi.bin
	cp kenshi.bin ./iso/boot/kenshi.bin
	grub-mkrescue -o kenshi.iso ./iso

kenshi.bin: $(LINK_LIST)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.as
	$(AS) -o $@ $<

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -rf src/*.o kenshi.bin kenshi.iso
