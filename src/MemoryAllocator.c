#include "../h/MemoryAllocator.h"

void* RESERVED_END_ADDR; 

void __MA_reserve_space() {
	uint64 all_bytes = HEAP_END_ADDR - HEAP_START_ADDR;
	uint64 res_bytes;
	uint64 div = 4*MEM_BLOCK_SIZE + 1;

	{//reserve the least amount of bytes needed to track all blocks
		uint64 tmp;
		tmp = all_bytes + (div - all_bytes % div);
		res_bytes = tmp / div;
	}
	
	RESERVED_END_ADDR = (void*)HEAP_START_ADDR + res_bytes;
	
	//align reserved space to a block
	RESERVED_END_ADDR += (MEM_BLOCK_SIZE - (uint64)RESERVED_END_ADDR % MEM_BLOCK_SIZE);
}
