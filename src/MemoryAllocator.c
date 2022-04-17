#include "../h/MemoryAllocator.h"

void memory_init() {
	struct Memory_block* bgn = (struct Memory_block*)HEAP_START_ADDR;
	bgn->next = 0;
	bgn->size = ((uint64)HEAP_END_ADDR-(uint64)HEAP_START_ADDR+1)/MEM_BLOCK_SIZE;
}

