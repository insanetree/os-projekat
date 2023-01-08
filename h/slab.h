#ifndef OS1_PROJEKAT_SLAB_H
#define OS1_PROJEKAT_SLAB_H

#include "../lib/hw.h"

struct slab {
	struct slab* next;
	void* spaceStartAddr;
};

typedef struct kmem_cache_s {
	const char* name;
	struct slab* empty;
	struct slab* partrial;
	struct slab* full;
	uint64 slotNum;
	uint64 slotFull;
	size_t slotSize;
	size_t slabSize;
	void (*ctor)(void*);
	void (*dtor)(void*);
} kmem_cache_t;
#define BLOCK_SIZE (32)

void kmem_init(void *space, int block_num);

kmem_cache_t *kmem_cache_create(const char *name, size_t size,
				void (*ctor)(void *),
				void (*dtor)(void *)); // Allocate cache
int kmem_cache_shrink(kmem_cache_t *cachep); // Shrink cache
void *kmem_cache_alloc(kmem_cache_t *cachep); // Allocate one object from cache
void kmem_cache_free(kmem_cache_t *cachep, void *objp); // Deallocate one object from cache
void *kmalloc(size_t size); // Alloacate one small memory buffer
void kfree(const void *objp); // Deallocate one small memory buffer
void kmem_cache_destroy(kmem_cache_t *cachep); // Deallocate cache
void kmem_cache_info(kmem_cache_t *cachep); // Print cache info
int kmem_cache_error(kmem_cache_t *cachep); // Print error message
#endif //OS1_PROJEKAT_SLAB_H
