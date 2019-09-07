#include <stdbool.h>
#include "char.h"
#include "port.h"
#include "serial.h"
#include "vga.h"

#define KEYBOARD_STATUS 0x64
#define KEYBOARD_DATA 0x60

// Virtual keys
typedef enum kb_vk {
	VK_ESC = 0x1B,
	VK_BACKSPACE = '\b',
	VK_TAB = '\t',
	VK_ENTER = '\n',
	VK_LCTRL = 0,
	VK_LSHFT,
	VK_RSHFT,
	VK_LALT,
	VK_CAPS,
	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
	VK_F9,
	VK_F10,
	VK_F11,
	VK_F12,
	VK_NUMLOCK,
	VK_SCROLLOCK,
} kb_vk;

bool shift_held = false;

char keymap[128] = {
    0,       VK_ESC, '1',        '2',          '3',    '4',   '5',      '6',      '7',   '8',      '9',
    '0',     '-',    '=',        VK_BACKSPACE, VK_TAB, 'q',   'w',      'e',      'r',   't',      'y',
    'u',     'i',    'o',        'p',          '[',    ']',   VK_ENTER, VK_LCTRL, 'a',   's',      'd',
    'f',     'g',    'h',        'j',          'k',    'l',   ';',      '\'',     '`',   VK_LSHFT, '\\',
    'z',     'x',    'c',        'v',          'b',    'n',   'm',      ',',      '.',   '/',      VK_RSHFT,
    '*', // keypad
    VK_LALT, ' ',    VK_CAPS,    VK_F1,        VK_F2,  VK_F3, VK_F4,    VK_F5,    VK_F6, VK_F7,    VK_F8,
    VK_F9,   VK_F10, VK_NUMLOCK, VK_SCROLLOCK, '7',    '8',   '9',      '-', // keypad
    '4',     '5',    '6',        '+',                                        // keypad
    '1',     '2',    '3',                                                    // keypad
    '0',     '.',                                                            // keypad
    VK_F11,  VK_F12,
};

static char translate(char c) {
	if (!shift_held)
		return c;

	char *digits = ")!@#$%^*(";

	if (isdigit(c)) {
		size_t offset = c - '0';
		return digits[offset];
	}

	if (isalpha(c)) {
		size_t offset = c - 'a';
		return 'A' + offset;
	}

	switch (c) {
		case '-':
			return '_';
		case '=':
			return '+';
		case '[':
			return '{';
		case ']':
			return '}';
		case ';':
			return ':';
		case '\'':
			return '"';
		case '\\':
			return '|';
		case ',':
			return '<';
		case '.':
			return '>';
		case '`':
			return '~';
	}

	return c;
}

// Keeping track of the last character entered via the keyboard.
static volatile char last_write = 0;
// Keeping track of whether kb_getc is currently waiting for a character to read.
static volatile bool pending = false;
// Keeping track of how many characters have been read with kb_gets, so that BACKSPACE
// can not go too far back.
static volatile size_t counter = 0;

// Handle keyboard IRQ
void keyboard_handler(void) {
	port_wb(0x20, 0x20); // EOI

	uint8_t status = port_rb(KEYBOARD_STATUS);
	if (status & 0x01) {
		uint8_t keycode = port_rb(KEYBOARD_DATA);
		switch (keycode) {
			// Check if left or right shift pressed
			case 0x2A:
			case 0x36: {
				shift_held = true;
				break;
			}
			// Check if left or right shift released
			case 0xAA:
			case 0xB6: {
				shift_held = false;
				break;
			}
			default: {
				if (!pending) {
					return;
				}

				char key = translate(keymap[keycode]);
				if (isprinting(key) || key == VK_ENTER || key == VK_BACKSPACE) {
					last_write = key;
					pending = false;
					if (key == VK_BACKSPACE && counter > 0) {
						vga_putc(key);
						counter--;
					} else if (key != VK_BACKSPACE) {
						vga_putc(key);
						counter++;
					}
				}
				break;
			}
		}
	}
}

static void kb_flush(void) {
	counter = 0;
}

char kb_getc(void) {
	pending = true;
	while (pending) {}
	return last_write;
}

size_t kb_gets(char *buf, size_t buf_size) {
	kb_flush();
	char *start = buf;
	char c;
	size_t len = 0;
	while ((c = kb_getc()) != '\n' && len < buf_size - 1) {
		if (c == '\b' && buf > start) {
			*buf = 0;
			buf--;
		} else {
			*buf = c;
			buf++;
			len++;
		}
	}
	kb_flush();
	return len;
}
