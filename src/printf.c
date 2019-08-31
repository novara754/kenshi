#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "stdio.h"
#include "vga.h"
#include "string.h"

static void printf_(const char *format, va_list args) {
	int ci = 0;
	int format_len = strlen(format);
	while (ci < format_len) {
		if (format[ci] == '%') {
			ci++;

			bool length = false;

			while (true) {
				if (format[ci] == 'z') {
					length = true;
					ci++;
				} else {
					break;
				}
			}

			switch (format[ci]) {
				case 'i': case 'd': {
					int32_t n;
					if (length) n = va_arg(args, size_t);
					else n = va_arg(args, int32_t);
					vga_puti(n);
					break;
				}
				case 'u': {
					uint32_t n;
					if (length) n = va_arg(args, size_t);
					else n = va_arg(args, uint32_t);
					vga_putui(n);
					break;
				}
				case 'x': {
					uint32_t n;
					if (length) n = va_arg(args, size_t);
					else n = va_arg(args, uint32_t);
					vga_puti_hex(n, true);
					break;
				}
				case 'X': {
					uint32_t n;
					if (length) n = va_arg(args, size_t);
					else n = va_arg(args, uint32_t);
					vga_puti_hex(n, true);
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

			ci++;
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
