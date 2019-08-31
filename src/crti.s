.section .init
.global _init
.type _init, @function
_init:
	push %ebp
	movl %esp, %ebp
	// gcc will fill in the rest with its crtbegin.o

.section .fini
.global _fini
.type _fini, @function
_fini:
	push %ebp
	movl %esp, %ebp
	// gcc will fill in the rest with its crtbegin.o
