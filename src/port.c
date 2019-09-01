#include "port.h"

void port_wb(uint16_t port, uint8_t data) {
	asm volatile ("outb  %%al, %%dx" : : "d" (port), "a" (data));
}

uint8_t port_rb(uint16_t port) {
	uint8_t ret;
	asm volatile ("inb %%dx, %%al" : "=a" (ret) : "d" (port));
	return ret;
}

void port_ww(uint16_t port, uint16_t data) {
	asm volatile ("outw  %%ax, %%dx" : : "d" (port), "a" (data));
}

uint16_t port_rw(uint16_t port) {
	uint16_t ret;
	asm volatile ("inw %%dx, %%ax" : "=a" (ret) : "d" (port));
	return ret;
}

void port_wl(uint16_t port, uint32_t data) {
	asm volatile ("outl  %%eax, %%dx" : : "d" (port), "a" (data));
}

uint32_t read_wl(uint16_t port) {
	uint32_t ret;
	asm volatile ("inl %%dx, %%eax" : "=a" (ret) : "d" (port));
	return ret;
}

