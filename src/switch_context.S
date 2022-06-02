.global __switch_context

__switch_context:
	sd ra, 0(a0)
	sd sp, 8(a0)

	ld ra, 0(a1)
	ld sp, 8(a1)

	ret
