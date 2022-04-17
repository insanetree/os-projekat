#ifndef _MEMORYALLOCATOR_H
#define _MEMORYALLOCATOR_H

#include "../lib/hw.h"

struct Memory_block {
	size_t size;
	struct Memory_block* next;
};

void memory_init();

void* block_alloc(size_t size);

void block_free(void* block, size_t size);

#endif //_MEMORYALLOCATOR_H
