#ifndef _MEMORYALLOCATOR_H
#define _MEMORYALLOCATOR_H

#include "../lib/hw.h"

extern void* FREE_SPACE_START;

void __MA_memory_init();
void* __MA_allocate(size_t size);

#endif //_MEMORYALLOCATOR_H
