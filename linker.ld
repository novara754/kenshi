/* Bootloader uses entry point as jump target */
ENTRY(_start)

SECTIONS
{
	/* 1MiB is a convenient location for kernels. */
	. = 1M;

	/* First we need the multiboot header, so the bootloader can recognize us.
	   The rest of the .text can follow. */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	/* Read-only data */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Initialized read-write data */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* Uninitialized read-write data */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
