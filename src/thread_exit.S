.global thread_exit

thread_exit:
	addi a0, zero, 0x12
	ecall
	ret