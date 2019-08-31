#ifndef KERNEL_VGA_H
#define KERNEL_VGA_H

#include <stdint.h>
#include <stddef.h>

#define VGA_FG(color) ((color) & 0xF)
#define VGA_BG(color) ((color) >> 4 & 0xF)

// Possible colours for VGA textmode.
typedef enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vga_color;

// Stores information about one cell in the VGA buffer.
typedef struct __attribute__((__packed__)) cell {
	unsigned char content;
	uint8_t color;
} cell;

// Initializes the VGA buffer by clearing it with spaces and setting the colour to white on blue.
void vga_init(void);
// Writes a character to the current cursor position.
void vga_putc(char c);
// Writes a null-terminated string.
void vga_puts(const char *str);
// Writes a base-10 signed 32-bit integer.
void vga_puti(int32_t n);
// Updates the cursor position.
void vga_setcursor(size_t row, size_t col);
// Gets the cursor row.
size_t vga_getcursor_row(void);
// Gets the cursor column.
size_t vga_getcursor_col(void);
// Gets the color  and content of the cell at the given position (zero-indexed).
struct cell* vga_getcell(size_t row, size_t col);

#endif
