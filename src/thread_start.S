.global thread_start

thread_start:
        mv a1, a0
        addi a0, zero, 0x14
        ecall
        ret