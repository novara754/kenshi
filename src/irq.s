.section .text

.global idt_load
.type idt_load, @function
idt_load:
	movl 4(%esp), %edx
	lidt (%edx)
	sti
	ret

.global irq_keyboard
.type irq_keyboard, @function
irq_keyboard:
	pusha
	call keyboard_handler
	popa
	iret
