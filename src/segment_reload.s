.section .text
.global segment_reload
.type segment_reload, @function
segment_reload:
	jmp $0x08, $reload_cs // 0x08 is the code selector (byte 8 of the gdt)
reload_cs:
	movw $0x10, %ax // 0x10 is the data selector (byte 16 of the gdt)
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	ret
