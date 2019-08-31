#include <stdarg.h>
#include <stdint.h>
#include "stdio.h"
#include "vga.h"
#include "string.h"

static void printf_(const char *format, va_list args) {
	int ci = 0;
	int format_len = strlen(format);
	while (ci < format_len) {
		if (format[ci] == '%' && ci < (format_len - 1)) {
			switch (format[ci + 1]) {
				case 'i': case 'u': {
					int32_t n = va_arg(args, int32_t);
					vga_puti(n);
					break;
				}
				case 'c': {
					unsigned char c = va_arg(args, int);
					vga_putc(c);
					break;
				}
				case 's': {
					const char *str = va_arg(args, const char *);
					vga_puts(str);
					break;
				}
				default: {
					vga_putc('%');
					vga_putc(format[ci + 1]);
					break;
				}
			}

			ci += 2;
		} else {
			vga_putc(format[ci]);
			ci++;
		}
	}
}

void printf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	printf_(format, args);
	va_end(args);
}
