// Options and constants for multiboot header.
.set ALIGN, 1 << 0 // Align on page boundaries.
.set MEMINFO, 1 << 1 // Enable memory map.
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

// Multiboot header has its own section
// such that it can be forced into the first 8 KiB.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384 // 16 KiB reserved for the stack.
stack_top:

// Defined as the entry point in the linker script.
// Bootloader will jump here.
.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp
	call kmain

	// Create infinite loop such that the operating system doesnt suddenly quit.
	cli
	// Although CLI disables the interrupts HLT waits for, there can still be cases in which
	// HLT doesn't halt forever. If this happens we just want to HLT again.
1:	hlt
	jmp 1b

// Set size of _start for later features such as debugging or call tracing.
.size _start, . - _start
