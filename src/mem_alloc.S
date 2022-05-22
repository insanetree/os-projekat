.global mem_alloc

mem_alloc:
	mv a1, a0
	addi a0, zero, 0x01
	ecall
	ret