#include "port.h"
#include "vga.h"
#include "serial.h"

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
	VK_NUMLOCK,
	VK_SCROLLOCK,
} kb_vk;

// TODO: Complete
char keymap[128] = {
	0, VK_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', VK_BACKSPACE,
	VK_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', VK_ENTER,
	VK_LCTRL, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	VK_LSHFT, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', VK_RSHFT,
	'*', // keypad
	VK_LALT,
	' ',
	VK_CAPS,
	VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10,
	VK_NUMLOCK, VK_SCROLLOCK,
};

// Handle keyboard IRQ
void keyboard_handler(void) {
	port_wb(0x20, 0x20); // EOI

	uint8_t status = port_rb(KEYBOARD_STATUS);
	if (status & 0x01) {
		int8_t keycode = port_rb(KEYBOARD_DATA);
		if (keycode < 0) return;
		vga_putc(keymap[keycode]);
	}
}
