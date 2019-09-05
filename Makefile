AS=i686-elf-as
CC=i686-elf-gcc
LD=i686-elf-gcc
ASFLAGS=-g3
CFLAGS=-c -Isrc/include -std=gnu99 -ffreestanding -Wall -Wextra -Werror -Wpedantic -g
LDFLAGS=-T linker.ld -ffreestanding -nostdlib -lgcc -g

CRTBEGIN=$(shell $(CC) $(CFLAGS) --print-file-name=crtbegin.o)
CRTEND=$(shell $(CC) $(CFLAGS) --print-file-name=crtend.o)

OBJECTS=src/kernel.o src/vga.o src/string.o src/printf.o \
		src/gdt.o src/segment_reload.o src/multiboot.o \
		src/port.o src/serial.o src/idt.o src/irq.o src/keyboard.o \
		src/char.o
HEADERS=$(wildcard src/*.h)

LINK_LIST=src/boot.o src/crti.o $(CRTBEGIN) $(OBJECTS) $(CRTEND) src/crtn.o

.PHONY: all
all: kenshi.iso

.PHONY: run
run: all
	qemu-system-i386 -cdrom kenshi.iso -serial stdio

kenshi.iso: kenshi.bin
	grub-file --is-x86-multiboot kenshi.bin
	cp kenshi.bin ./iso/boot/kenshi.bin
	grub-mkrescue -o kenshi.iso ./iso

kenshi.bin: $(LINK_LIST)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.as
	$(AS) $(ASFLAGS) -o $@ $<

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -rf src/*.o kenshi.bin kenshi.iso
