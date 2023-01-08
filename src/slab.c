#include "../h/slab.h"
#include "../h/buddy.h"

kmem_cache_t *kmem_cache_create(const char *name, size_t objSize,
				void (*ctor)(void *),
				void (*dtor)(void *)) {
	kmem_cache_t* cache = buddy_allocate(sizeof(kmem_cache_t));
	if(cache == 0)
		return 0;
	cache->name = name;
	cache->ctor = ctor;
	cache->dtor = dtor;
	cache->empty = 0;
	cache->partrial = 0;
	cache->full = 0;
	cache->slotFull = 0;
	cache->slotNum = 0;
	cache->slotSize = objSize;
	return cache;
}

int kmem_cache_shrink(kmem_cache_t *cachep) {
	if(cachep == 0) {
		return -1;
	}
	int blocksFreed=0;
	struct slab* cur = cachep->empty;
	struct slab* prev;
	while(cur) {
		prev = cur;
		cur = cur->next;
		buddy_free(prev, cachep->slabSize);
		blocksFreed += cachep->slabSize;
	}
	return blocksFreed / BLOCK_SIZE;
}