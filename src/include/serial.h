#pragma once

#include <stdbool.h>
#include <stdint.h>

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
void serial_writes(com_port port, const char *str);
uint8_t serial_read(com_port port);
