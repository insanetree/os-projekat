.global putc

putc:
	mv a1, a0
	addi a0, zero, 0x42
	ecall
	ret