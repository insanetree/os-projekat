#include "../h/slab.h"
#include "../h/buddy.h"
#include "../h/MemoryAllocator.h"

void clear_slab_list(struct slab* list, size_t slabSize);

kmem_cache_t *kmem_cache_create(const char *name, size_t objSize,
				void (*ctor)(void *),
				void (*dtor)(void *)) {
	kmem_cache_t* cache = __MA_allocate(sizeof(kmem_cache_t));
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

/**
 * worry about:
               when to go partial->full
               increase slotFull
               when to increase slotNum
 * @param cachep
 * @return
 */
void *kmem_cache_alloc(kmem_cache_t *cachep) {
	struct slab* head;
	if(cachep->partrial == 0 && cachep->empty == 0) {
		head = buddy_allocate(cachep->slabSize);
		head->spaceStartAddr = (void*)head + sizeof(struct slab);
		head->next = 0;
		cachep->partrial = head;
		head->slotsBitmask = 0;
		cachep->slotNum += cachep->slotsInSlab;
	} else if(cachep->partrial == 0 && cachep->empty != 0) {
		head = cachep->empty;
		cachep->empty = cachep->empty->next;
		cachep->partrial = head;
	} else {
		head = cachep->partrial;
	}
	cachep->slotFull++;
	uint8 freeSlot = 0;
	while((1<<freeSlot) & head->slotsBitmask && freeSlot < cachep->slotsInSlab)
		freeSlot++;
	void* slotAddress = head->spaceStartAddr + (freeSlot * cachep->slotSize);
	head->slotsBitmask |= (1<<freeSlot);
	if(head->slotsBitmask == (1<<cachep->slotsInSlab) - 1) {
		cachep->partrial = head->next;
		head->next = cachep->full;
		cachep->full = head;
	}
	return slotAddress;
}

void kmem_cache_destroy(kmem_cache_t *cachep) {
	clear_slab_list(cachep->empty, cachep->slabSize);
	clear_slab_list(cachep->full, cachep->slabSize);
	clear_slab_list(cachep->partrial, cachep->slabSize);
	__MA_free(cachep);
}

void clear_slab_list(struct slab* list, size_t slabSize) {
	if(list == 0)
		return;
	struct slab* cur = list;
	struct slab* prev;
	while(cur) {
		prev = cur;
		cur = cur->next;
		buddy_free(prev, slabSize);
	}
}