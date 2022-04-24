#include "../h/MemoryAllocator.h"
#include "../h/defines.h"

void* RESERVED_END_ADDR; 
void* FREE_SPACE_START;

struct __MA_memory_block {
	size_t size;
	struct __MA_memory_block* next;
};


/**
 * This function reserves space for monitoring allocated memory
 * After this RESERVED_END_ADDR will become a start addr for heap memory
 * Function MUST be run first to initialize memory
 */
void __MA_reserve_space() {
	uint64 all_bytes = HEAP_END_ADDR - HEAP_START_ADDR;
	uint64 res_bytes;
	const uint8 two_bit_sections = 4;
	uint64 div = two_bit_sections*MEM_BLOCK_SIZE + 1;

	{//reserve the least amount of bytes needed to track all blocks
		uint64 tmp;
		tmp = all_bytes + (div - all_bytes % div);
		res_bytes = tmp / div;
	}
	
	RESERVED_END_ADDR = (void*)HEAP_START_ADDR + res_bytes;
	
	//align reserved space to a block
	RESERVED_END_ADDR += (MEM_BLOCK_SIZE - (uint64)RESERVED_END_ADDR % MEM_BLOCK_SIZE);
}

void __MA_memory_init() {
	size_t block_size;
	struct __MA_memory_block* start;
	__MA_reserve_space();
	FREE_SPACE_START = RESERVED_END_ADDR;
	start = (struct __MA_memory_block*)FREE_SPACE_START;
	block_size = ((uint64)HEAP_END_ADDR - (uint64)FREE_SPACE_START);
	start->size = block_size;
	start->next = NULL;
}
