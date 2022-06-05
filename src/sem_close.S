.global sem_close

sem_close:
	mv a1, a0
	addi a0, zero, 0x22
	ecall
	ret