AS=i686-elf-as
CC=i686-elf-gcc
LD=i686-elf-gcc
CFLAGS=-c -Isrc/include -std=gnu99 -ffreestanding -Wall -Wextra -Werror -Wpedantic
LDFLAGS=-T linker.ld -ffreestanding -nostdlib -lgcc
LINK_LIST=src/boot.o src/kernel.o src/vga.o
HEADERS=$(wildcard src/*.h)

.PHONY: all
all: kenshi.bin

kenshi.bin: $(LINK_LIST)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.as
	$(AS) -o $@ $<

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -rf src/*.o kenshi.bin
