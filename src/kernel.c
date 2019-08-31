#include "stdio.h"
#include "vga.h"
#include "gdt.h"

void kmain_early(void) {
	vga_init();
	printf("[VGA] Initializing VGA terminal... Done\n");

	// null segment, not used by cpu
	gdt_set_entry(0, 0, 0, 0);
	// kernel code segment
	gdt_set_entry(1, 0, 0xFFFFFFFF, GDTA_READWRITE | GDTA_EXECUTABLE | GDTA_NOTSYSTEM | GDTA_PRESENT);
	// kernel data segment
	gdt_set_entry(2, 0, 0xFFFFFFFF, GDTA_READWRITE | GDTA_NOTSYSTEM | GDTA_PRESENT);
	// tss
	// gdt_set_entry(3, &tss, sizeof(tss), GDTA_ACCESSED | GDTA_EXECUTABLE | GDTA_PRESENT);
	gdt_load();
}

void kmain(void) {
	printf("\nKenshi is initialized and ready to go.\n\n");
	printf("1 + 2 = %i\n", 1 + 2);
	printf("3 - 5 = %i\n", 3 - 5);
	char *text = "hello, world!";
	printf("The first char of \"%s\" is '%c'.\n", text, text[0]);
}
