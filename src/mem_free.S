.global mem_free

mem_free:
	mv a1, a0
	addi a0, zero, 0x02
	ecall
	ret