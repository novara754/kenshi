#include "vga.h"

void kmain(void) {
	vga_init();
	vga_puts("Hello, world!\n");
	cell *c = vga_getcell(0, 0);
	vga_putc(c->content);

	vga_setcursor(10, 0);
	vga_puts("Goodbye, world...");
}
