#ifndef _MEMORYALLOCATOR_H
#define _MEMORYALLOCATOR_H

#include "../lib/hw.h"

#ifdef __cplusplus
extern "C" {
#endif
extern void* FREE_SPACE_START;

void __MA_memory_init();
void* __MA_allocate(size_t size);
int __MA_free(void* ptr);

#ifdef __cplusplus
}
#endif


#endif //_MEMORYALLOCATOR_H
