.section .init
	popl %ebp
	ret
	// gcc will fill in the rest with its crtend.o

.section .fini
	popl %ebp
	ret
	// gcc will fill in the rest with its crtend.o
