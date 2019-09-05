#include <stdint.h>
#include <stddef.h>
#include "idt.h"
#include "port.h"
#include "stdio.h"

/* Definitions in irq.s */
extern int idt_load();
extern int irq_keyboard();

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

typedef struct __attribute__((packed)) idt_gate {
	uint16_t offset_lo;
	uint16_t selector;
	uint8_t reserved;
	uint8_t type;
	uint16_t offset_hi;
} idt_gate;

idt_gate idt[IDT_GATES];

void idt_set(size_t index, int (*handler)(void), uint8_t type) {
	uint32_t h = (uint32_t)handler;

	idt_gate gate = {
		.offset_lo = h & 0xFFFF,
		.offset_hi = (h >> 16) & 0xFFFF,
		.type = type,
		.reserved = 0,
		.selector = 0x08,
	};

	idt[index] = gate;
}

void idt_init(void) {
	printf("[IDT] Initializing IDT... ");

	// Reprogramming the PICs

	// 0x11 = Init Command Word 1
	// After ICW 1 the PICs expect 3 bytes of config commands.
	port_wb(PIC1_COMMAND, 0x11);
	port_wb(PIC2_COMMAND, 0x11);

	// ICW 2 : Vector offset
	// Has to be remapped because in x86 the default offsets (up to 0x20) are reserved
	// for CPU exceptions.
	// Start PIC 1 (IRQ 0~7) at offset 32 and PIC 2 (IRQ 8~15)
	// at offset 40.
	port_wb(PIC1_DATA, 0x20);
	port_wb(PIC2_DATA, 0x28);

	// ICW 3 : Cascading
	// Disabled
	port_wb(PIC1_DATA, 0x00);
	port_wb(PIC2_DATA, 0x00);

	// ICW 4 : Environment
	// Just tell PIC we're running in 80x86 mode
	port_wb(PIC1_DATA, 0x01);
	port_wb(PIC2_DATA, 0x01);

	// Set interrupt mask to only allow keyboard (irq 1) for now
	port_wb(PIC1_DATA, 0xFD);
	port_wb(PIC2_DATA, 0x00);

	/* Keyboard */
	idt_set(33, irq_keyboard, 0x8E);

	struct __attribute__((packed)) idtd {
		uint16_t size;
		uint32_t addr;
	} idtd = {
		.size = sizeof(idt_gate) * IDT_GATES,
		.addr = (uint32_t)idt,
	};
	idt_load(&idtd);

	printf("Done\n");
}
