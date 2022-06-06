.global sem_wait

sem_wait:
	mv a1, a0
	addi a0, zero, 0x23
	ecall
	ret