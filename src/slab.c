#include "../h/slab.h"
#include "../h/buddy.h"
#include "../h/MemoryAllocator.h"

void clear_slab_list(struct slab* list, size_t slabSize);

/**
 *
 * @param objp
 * @param cachep
 * @param prev
 * @param slab
 * @return 0 - object was in partially full list, 1 - object was in full list
 */
int find_object(void* objp, kmem_cache_t *cachep, struct slab** prev, struct slab** slab);

uint8 slotsInSlab(size_t objSize);

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
	cache->slotsInSlab= slotsInSlab(cache->slotSize);
	return cache;
}

int kmem_cache_shrink(kmem_cache_t *cachep) {
	if(cachep == 0) {
		return -1;
	}
	int blocksFreed=0;
	struct slab* cur = cachep->empty;
	struct slab* prev;
	size_t slabSize = cachep->slotSize*cachep->slotsInSlab;
	while(cur) {
		prev = cur;
		cur = cur->next;
		cachep->slotNum -= cachep->slotsInSlab;
		buddy_free(prev->spaceStartAddr,slabSize);
		buddy_free(prev, sizeof(struct slab));
		blocksFreed += sizeof(struct slab)+slabSize;
	}
	return blocksFreed / BLOCK_SIZE;
}

/**
 * worry about:
               when to go partial->full done
               increase slotFull done
               when to increase slotNum done
 * @param cachep
 * @return
 */
void *kmem_cache_alloc(kmem_cache_t *cachep) {
	struct slab* head;
	size_t slabSize = cachep->slotSize*cachep->slotsInSlab;
	if(cachep->partrial == 0 && cachep->empty == 0) {
		head = buddy_allocate(sizeof(struct slab));
		head->spaceStartAddr = buddy_allocate(slabSize);
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
	if(cachep->ctor) {
		cachep->ctor(slotAddress);
	}
	return slotAddress;
}

void kmem_cache_destroy(kmem_cache_t *cachep) {
	size_t slabSize;
	slabSize = cachep->slotSize * cachep->slotsInSlab;
	clear_slab_list(cachep->empty, slabSize);
	clear_slab_list(cachep->full, slabSize);
	clear_slab_list(cachep->partrial, slabSize);
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
		buddy_free(prev->spaceStartAddr, slabSize);
		buddy_free(prev, sizeof(struct slab));
	}
}

uint8 slotsInSlab(size_t objSize) {
	return 8;
}

/**
 * worry about
 * full->partial
 * partial->empty
 * slotFull--
 * @param cachep
 * @param objp
 */
void kmem_cache_free(kmem_cache_t *cachep, void *objp) {
	struct slab *cur, *prev, **headTo, **headFrom;
	int ret;
	uint64 slotIndex;
	ret = find_object(objp, cachep, &prev, &cur);
	headFrom = (ret) ? (&(cachep->full)) : (&(cachep->partrial));
	headTo = (ret) ? (&(cachep->partrial)) : (&(cachep->empty));
	cachep->slotFull--;
	slotIndex = (uint64)objp - (uint64)cur->spaceStartAddr;
	slotIndex /= cachep->slotSize;
	cur->slotsBitmask &= ~(1UL<<slotIndex);
	if(cachep->dtor)
		cachep->dtor(objp);
	if(ret || cur->slotsBitmask == 0) {
		if(prev) prev->next = cur->next;
		else *headFrom = cur->next;
		cur->next = *headTo;
		*headTo = cur;
	}
}

int find_object(void* objp, kmem_cache_t* cachep, struct slab** previous, struct slab** slabWhereFound) {
	struct slab *cur, *prev;
	size_t slabSize;
	slabSize=cachep->slotsInSlab*cachep->slotSize;
	prev=0;
	cur=cachep->partrial;
	while(cur){
		if(objp >= cur->spaceStartAddr && objp < cur->spaceStartAddr+slabSize) {
			*previous = prev;
			*slabWhereFound = cur;
			return 0;
		}
		prev = cur;
		cur = cur->next;
	}
	prev=0;
	cur=cachep->full;
	while(cur) {
		if(objp >= cur->spaceStartAddr && objp < cur->spaceStartAddr+slabSize) {
			*previous = prev;
			*slabWhereFound = cur;
			return 1;
		}
		prev = cur;
		cur = cur->next;
	}
	return -1;
}