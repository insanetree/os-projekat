.global time_sleep

time_sleep:
	mv a1, a0
	addi a0, zero, 0x31
	ecall
	ret