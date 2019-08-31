#include "vga.h"

void kmain(void) {
	vga_init();

	for (int i = -5; i <= 5; i++) {
		vga_puti(i);
		vga_putc('\n');
	}
}
