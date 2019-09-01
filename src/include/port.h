#ifndef KERNEL_PORT_H
#define KERNEL_PORT_H

#include <stdint.h>

void port_wb(uint16_t port, uint8_t data);
uint8_t port_rb(uint16_t port);

void port_ww(uint16_t port, uint16_t data);
uint16_t port_rw(uint16_t port);

void port_wl(uint16_t port, uint32_t data);
uint32_t port_rl(uint16_t port);

#endif
