#include "stdio.h"
#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "multiboot.h"
#include "serial.h"

void kmain_early(void) {
	vga_init();
	printf("[VGA] Initializing VGA terminal... Done\n");

	serial_init(COM1, BAUD_9600);

	// null segment, not used by cpu
	gdt_set_entry(0, 0, 0, 0);
	// kernel code segment
	gdt_set_entry(1, 0, 0xFFFFFFFF, GDTA_READWRITE | GDTA_EXECUTABLE | GDTA_NOTSYSTEM | GDTA_PRESENT);
	// kernel data segment
	gdt_set_entry(2, 0, 0xFFFFFFFF, GDTA_READWRITE | GDTA_NOTSYSTEM | GDTA_PRESENT);
	// tss
	// gdt_set_entry(3, &tss, sizeof(tss), GDTA_ACCESSED | GDTA_EXECUTABLE | GDTA_PRESENT);
	gdt_load();

	idt_init();
}

void kmain(multiboot_info *mb) {
	printf("\nKenshi is initialized and ready to go.\n");
	if (mb_flags(mb, MB_BL_NAME)) {
		printf("Booted using %s.\n", mb->boot_loader_name);
	} else {
		printf("Boot loader could not be identified.\n");
	}
	if (mb_flags(mb, MB_MEM)) {
		printf("Lower memory: %luKB -- Upper memory: %luKB\n", mb->mem_lower, mb->mem_upper);
	}

	printf("You can do some basic typing here:\n");

	// Endless loop in which interrupts can happen.
	while (true);
}
