#include <stdbool.h>
#include "vga.h"
#include "serial.h"

static volatile uint16_t *VGA_BUFFER = (uint16_t*)0xB8000;
const size_t VGA_HEIGHT = 25;
const size_t VGA_WIDTH = 80;

static size_t row;
static size_t col;
static uint8_t color;

// Creates an entry as expected by the VGA text mode.
static inline uint16_t make_entry(unsigned char c) {
	return (uint16_t)c | (uint16_t)color << 8;
}

// Creates a color for a cell as expected by the VGA text mode.
static inline uint8_t make_color(vga_color fg, vga_color bg) {
	return fg | bg << 4;
}

// Moves every line up by one line, creating a new empty one at the bottom.
static void scroll(void) {
	for (size_t y = 1; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = x + y * VGA_WIDTH;
			const size_t above_index = x + (y-1) * VGA_WIDTH;
			VGA_BUFFER[above_index] = VGA_BUFFER[index];
		}
	}

	// After scrolling we also need to clear the last row, otherwise it will be there twice.
	for (size_t x = 0; x < VGA_WIDTH; x++) {
		const size_t index = x + (VGA_HEIGHT - 1) * VGA_WIDTH;
		VGA_BUFFER[index] = make_entry(' ');
	}
}

// Properly handles line-breaks (i.e., scrolls when necessary).
static void newline(void) {
	col = 0;
	if (++row >= VGA_HEIGHT) {
		scroll();
		row = VGA_HEIGHT - 1;
	}
}

void vga_init(void) {
	row = 0;
	col = 0;
	color = make_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = x + y * VGA_WIDTH;
			VGA_BUFFER[index] = make_entry(' ');
		}
	}
}

void vga_putc(char c) {
	switch (c) {
		case '\n': {
			newline();
			break;
		}
		case '\t': {
			// Calculate the next cell on an 8-cell boundary.
			size_t rest = col % 8;
			col += 8 - rest;
			break;
		}
		case '\b': {
			if (col > 0) {
				col -= 1;
			} else {
				if (row > 0) {
					row -= 1;
					col = VGA_WIDTH - 1;
				}
			}

			const size_t index = col + row * VGA_WIDTH;
			VGA_BUFFER[index] = make_entry(' ');
			break;
		}
		default: {
			const size_t index = col + row * VGA_WIDTH;
			VGA_BUFFER[index] = make_entry(c);

			if (++col >= VGA_WIDTH) {
				newline();
			}
			break;
		}
	}
}

void vga_puts(const char *str) {
	for (size_t i = 0; str[i]; i++) {
		vga_putc(str[i]);
	}
}

void vga_puti(int32_t n) {
	// If the number is negative we want to prefix it with a '-' and
	// then pretend it was actually a positive number.
	if (n < 0) {
		vga_putc('-');
		n *= -1;
	}

	vga_putui(n);
}

void vga_putui(uint32_t n) {
	// n=0 is a special case and won't work with the following algorithm.
	if (n == 0) {
		vga_putc('0');
		return;
	}

	// To be able to print the number we need to first reverse it.
	// n % 10 essentially gives us the last digit of the number.
	// Using this "trick" we can reverse the number here, and then use the same "trick"
	// later when printing it.
	int32_t rev_n = 0;
	while (n > 0) {
		rev_n = (rev_n * 10) + (n % 10);
		n /= 10;
	}

	while (rev_n) {
		int d = rev_n % 10;
		unsigned char c = '0' + d;
		vga_putc(c);
		rev_n /= 10;
	}
}

void vga_puti_hex(uint32_t n, bool capitals) {
	if (n == 0) {
		vga_putc('0');
		return;
	}

	bool start = true;
	for (int shift = 28; shift >= 0; shift -= 4) {
		int d = (n >> shift) & 0xF;
		if (d == 0 && start) continue;
		start = false;
		unsigned char c;
		if (d < 10) c = '0' + d;
		else c = (capitals ? 'A' : 'a') + d - 10;
		vga_putc(c);
	}
}

void vga_setcursor(size_t r, size_t c) {
	row = r;
	col = c;
}

inline size_t vga_getcursor_row(void) {
	return row;
}

inline size_t vga_getcursor_col(void) {
	return col;
}

inline cell* vga_getcell(size_t row, size_t col) {
	const size_t index = col + row * VGA_WIDTH;
	return (cell*)&VGA_BUFFER[index];
}