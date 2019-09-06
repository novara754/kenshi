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

static char add_shift(char c) {
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
				char key = keymap[keycode];
				if (!isprinting(key) && key != VK_ENTER && key != VK_BACKSPACE && key != VK_TAB) {
					return;
				}
				if (shift_held) {
					key = add_shift(key);
				}
				vga_putc(key);
				break;
			}
		}
	}
}
