#ifndef KERNEL_SERIAL_H
#define KERNEL_SERIAL_H

#include <stdbool.h>

typedef enum com_port {
	COM1 = 0x3F8,
	COM2 = 0x2F8,
	COM3 = 0x3E8,
	COM4 = 0x2E8,
} com_port;

typedef enum baud_rate {
	BAUD_9600 = 9600,
} baud_rate;

void serial_init(com_port port, baud_rate baud);
void serial_write(com_port port, uint8_t data);
uint8_t serial_read(com_port port);

#endif
