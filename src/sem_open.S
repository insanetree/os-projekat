.global sem_open

sem_open:
	mv a2, a1
	mv a1, a0
	addi a0, zero, 0x21
	ecall
	ret