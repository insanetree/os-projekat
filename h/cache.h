#ifndef OS1_PROJEKAT_CACHE_H
#define OS1_PROJEKAT_CACHE_H

#include "slab.h"

#ifdef __cplusplus
extern "C" {
#endif
	extern kmem_cache_t* tcb_cache;
	extern kmem_cache_t* stack_cache;
	extern kmem_cache_t* semaphore_cache;
#ifdef __cplusplus
}
#endif

#endif //OS1_PROJEKAT_CACHE_H
