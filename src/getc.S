.global getc

getc:
	addi a0, zero, 0x41
	ecall
	ret