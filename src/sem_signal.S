.global sem_signal

sem_signal:
	mv a1, a0
	addi a0, zero, 0x24
	ecall
	ret