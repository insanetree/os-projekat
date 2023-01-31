#ifndef OS1_PROJEKAT_BUDDY_H
#define OS1_PROJEKAT_BUDDY_H

#include "MemoryAllocator.h"

#ifdef __cplusplus
extern "C" {
#endif
extern void* BUDDY_START_ADDR;
extern void* BUDDY_END_ADDR;

void buddy_init();
void* buddy_allocate(size_t size);
void buddy_free(void* address, size_t size);

#ifdef __cplusplus
}
#endif
#endif //OS1_PROJEKAT_BUDDY_H
