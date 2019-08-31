#include "stdio.h"
#include "vga.h"

void kmain(void) {
	vga_init();
	printf("1 + 2 = %i\n", 1 + 2);
	printf("3 - 5 = %i\n", 3 - 5);
	char *text = "hello, world!";
	printf("The first char of \"%s\" is '%c'.\n", text, text[0]);
}
