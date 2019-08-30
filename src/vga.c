#include "vga.h"

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
}

// Properly handles line-breaks (i.e., scrolls when necessary).
static void newline(void) {
	col = 0;
	if (++row >= VGA_HEIGHT) {
		scroll();
	}
}

void vga_init(void) {
	row = 0;
	col = 0;
	color = make_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLUE);
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = x + y * VGA_WIDTH;
			VGA_BUFFER[index] = make_entry(' ');
		}
	}
}

void vga_putc(char c) {
	if (c != '\n') {
		const size_t index = col + row * VGA_WIDTH;
		VGA_BUFFER[index] = make_entry(c);

		if (++col >= VGA_WIDTH) {
			newline();
		}
	} else {
		newline();
	}
}

void vga_puts(const char *str) {
	for (size_t i = 0; str[i]; i++) {
		vga_putc(str[i]);
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