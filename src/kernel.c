#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "multiboot.h"
#include "serial.h"
#include "stdio.h"
#include "string.h"
#include "vga.h"

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

	printf("\nEnter `help' for a list of commands.\n");
	while (true) {
		char buffer[80] = {0};
		printf("> ");
		kb_gets(buffer, 80);
		char *command = strtok(buffer, " \n");
		if (strcmp(command, "help") == 0) {
			printf("Available commands: help, echo, cls.\n");
		} else if (strcmp(command, "cls") == 0) {
			vga_init();
		} else if (strcmp(command, "echo") == 0) {
			char *text = command + 5;
			printf("%s\n", text);
		} else {
			printf("Unknown command `%s'.\n", command);
			printf("Enter `help' for a list of commands.\n");
		}
	}
}
