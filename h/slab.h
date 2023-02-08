#ifndef OS1_PROJEKAT_SLAB_H
#define OS1_PROJEKAT_SLAB_H

#include "../lib/hw.h"
#include "kernellib.h"
#ifdef __cplusplus
extern "C" {
#endif
struct slab {
	struct slab* next;
	uint64 slotsBitmask;
	void* spaceStartAddr;
};

typedef struct kmem_cache_s {
	const char* name;
	struct slab* empty;
	struct slab* partrial;
	struct slab* full;
	uint8 slotsInSlab;
	uint64 slotNum;
	uint64 slotFull;
	size_t slotSize;
	const char* errorMessage;
	void (*ctor)(void*);
	void (*dtor)(void*);
	struct kmem_cache_s* next;
	struct kmem_cache_s* prev;
} kmem_cache_t;
#define BLOCK_SIZE (32)

void kmem_init(void *space, int block_num);

kmem_cache_t *kmem_cache_create(const char *name, size_t size,
				void (*ctor)(void *),
				void (*dtor)(void *)); // Allocate cache DONE
int kmem_cache_shrink(kmem_cache_t *cachep); // Shrink cache DONE
void *kmem_cache_alloc(kmem_cache_t *cachep); // Allocate one object from cache DONE
void kmem_cache_free(kmem_cache_t *cachep, void *objp); // Deallocate one object from cache DONE
void *kmalloc(size_t size); // Alloacate one small memory buffer DONE
void kmfree(const void *objp); // Deallocate one small memory buffer
void kmem_cache_destroy(kmem_cache_t *cachep); // Deallocate cache DONE
void kmem_cache_info(kmem_cache_t *cachep); // Print cache info
int kmem_cache_error(kmem_cache_t *cachep); // Print error message
#ifdef __cplusplus
}
#endif

#endif //OS1_PROJEKAT_SLAB_H
