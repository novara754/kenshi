#include <stdint.h>
#include <stddef.h>
#include "serial.h"
#include "port.h"

// Internal clock rate of serial controller (UART).
// Used to calculate divisor for baud rate.
const uint32_t CLOCK_RATE = 115200;

void serial_init(com_port port, baud_rate baud) {
	uint16_t divisor = CLOCK_RATE / baud;

	// Set DLAB (MSB of Line Control Register) in order to set clock rate divisor.
	port_wb(port + 3, 0x80);
	// Because of the DLAB, port offset 0 and 1 now are used to set the lower and higher byte
	// for the clock rate divisor.
	port_wb(port + 0, divisor & 0xFF);
	port_wb(port + 1, divisor >> 8);

	// Interrupt Enable Register
	// Zeroed, interrupts are disabled.
	port_wb(port + 1, 0x00);

	// FIFO Control Register
	//   0 = Enable FIFO
	//   1 = Clear receive FIFO
	//   2 = Clear transmit FIFO
	//   3 = Select DMA Mode (0 select)
	//   4 = Reserved
	//   5 = Reserved
	// 6~7 = 14 byte threshold for interrupt trigger level.
	port_wb(port + 2, 0xC7);

	// Line Control Register
	// 0~1 = Number of data bits, set to 7 (01) for ASCII support
	//   2 = Stop bits, set to just 1 stop bit
	// 3~5 = Parity, set to NONE
	//   6 = Break signal, disabled
	//   7 = DLAB, disabled to allow use of data register
	port_wb(port + 3, 0x02);

	// Modem Control Register
	//   0 = Data terminal ready
	//   1 = Request to send
	//   2 = Auxiliary output 1
	//   3 =       -"-        2
	//   4 = Loopback mode
	//   5 = Autoflow control
	// 6~7 = Reserved
	port_wb(port + 4, 0x0B);
}

static bool serial_canwrite(com_port port) {
	// When data can be sent, the 5th bit of the Line Status Register is set
	return port_rb(port + 5) & 0x20;
}

void serial_write(com_port port, uint8_t data) {
	while (!serial_canwrite(port));
	port_wb(port + 0, data);
}

void serial_writes(com_port port, const char *str) {
	for (size_t i = 0; str[i]; i++) {
		serial_write(port, str[i]);
	}
}

static bool serial_hasdata(com_port port) {
	// When data is ready to be read, the LSB of the Line Status Register is set
	return port_rb(port + 5) & 1;
}

uint8_t serial_read(com_port port) {
	while (!serial_hasdata(port));
	return port_rb(port + 0);
}
