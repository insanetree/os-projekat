.global thread_create

thread_create:
	mv a3, a2
	mv a2, a1
	mv a1, a0
	addi a0, zero, 0x11
	ecall
	ret