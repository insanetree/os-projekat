#include "../h/slab.h"
#include "../h/cache.h"
#include "../h/buddy.h"
#include "../h/MemoryAllocator.h"
#include "../h/print.h"

kmem_cache_t* cache_head=NULL;
kmem_cache_t* cache_tail=NULL;

const char* noSpaceError = "No available space for slab allocation";
const char* objectNotInCache = "Object ould not be deallocated because it is not in selected cache";

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
void* BUDDY_START_ADDR;
void* BUDDY_END_ADDR;
uint64 minBuddySize;
uint64 numOfBuckets;
uint64 buddyBlockNum;
uint64 buddyBlockFree;
void kmem_init(void *space, int block_num) {
	BUDDY_START_ADDR=space;
	BUDDY_END_ADDR=BUDDY_START_ADDR+block_num*BLOCK_SIZE;
	buddyBlockNum = buddyBlockFree = block_num;
	uint64 tmp = BLOCK_SIZE;
	minBuddySize = 0;
	while(tmp%2 == 0) {
		minBuddySize++;
		tmp/=2;
	}
	numOfBuckets=1;
	while (block_num%2 == 0){
		numOfBuckets++;
		block_num/=2;
	}

	buddy_init();
}


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
	cache->slotsInSlab= slotsInSlab(cache->slotSize);
	cache->errorMessage=NULL;
	if(!cache_head) {
		cache_head=cache_tail = cache;
		cache->next=cache->prev = NULL;
	} else {
		cache->prev = cache_tail;
		cache->next = NULL;
		cache_tail->next = cache;
		cache_tail = cache;
	}
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
	cachep->empty=NULL;
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
		if(!head) {
			cachep->errorMessage = noSpaceError;
			return NULL;
		}

		head->spaceStartAddr = buddy_allocate(slabSize);
		if(!head->spaceStartAddr) {
			cachep->errorMessage = noSpaceError;
			return NULL;
		}
		head->next = NULL;
		cachep->partrial = head;
		head->slotsBitmask = 0;
		cachep->slotNum += cachep->slotsInSlab;
	} else if(cachep->partrial == 0 && cachep->empty != 0) {
		head = cachep->empty;
		cachep->empty = cachep->empty->next;
		head->next = NULL;
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
	if(cachep->next) cachep->next->prev = cachep->prev;
	if(cachep->prev) cachep->prev->next = cachep->next;
	if(cachep==cache_tail) cache_tail = cachep->prev;
	buddy_free(cachep, sizeof(kmem_cache_t));
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
	uint8 numOld=0, numNew = 8;
	int bucketOld, bucketNew;
	do {
		numOld = numNew;
		bucketOld = buddy_get_bucket(objSize * numOld);
		numNew++;
		bucketNew = buddy_get_bucket(objSize * numNew);
	} while(bucketNew == bucketOld && bucketOld > 0);
	return numOld;
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
	static struct slab *cur, *prev, **headTo, **headFrom;
	int ret;
	uint64 slotIndex;
	ret = find_object(objp, cachep, &prev, &cur);
	if(ret < 0){
		cachep->errorMessage=objectNotInCache;
		return;
	}
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

void* kmalloc(size_t size) {
	int bucket = buddy_get_bucket(size);
	return kmem_cache_alloc(buffer_cache[bucket]);
}

void kmfree(const void* objp) {
	const char* ermsg;
	for(int i = 0 ; i < 13 ; i++) {
		ermsg = buffer_cache[i]->errorMessage;
		kmem_cache_free(buffer_cache[i], (void *) objp);
		buffer_cache[i]->errorMessage = ermsg;
	}
}

void kmem_cache_info(kmem_cache_t* cachep) {
	uint64 spaceAllocated = 0;
	uint64 numOfSlabs;
	numOfSlabs = cachep->slotNum/cachep->slotsInSlab;
	spaceAllocated += 1<<(buddy_get_bucket(sizeof(kmem_cache_t))+minBuddySize);
	spaceAllocated += numOfSlabs*((1<<(buddy_get_bucket(sizeof(struct slab))+minBuddySize))
		+ (1<<(buddy_get_bucket(cachep->slotsInSlab*cachep->slotSize))));
	printstr(cachep->name);
	printstr(" object size:");
	printunum(cachep->slotSize);
	printstr(" blocks allocated:");
	printunum(spaceAllocated/BLOCK_SIZE);
	printstr(" number of slabs:");
	printunum(numOfSlabs);
	printstr(" space used:");
	printunum(cachep->slotFull * 100 / cachep->slotNum);
	printstr("%\n");
}

int kmem_cache_error(kmem_cache_t* cachep) {
	if(!cachep->errorMessage)
		return 0;
	printstr(cachep->errorMessage);
	return 132;
}

void shrink_caches() {
	if(100 * buddyBlockFree / buddyBlockNum > 60)
		return;
	kmem_cache_t* cur;
	uint64 threshold, percentFull;
	for(cur=cache_head ; cur ; cur=cur->next) {
		if(cur->slotNum == 0) continue;
		threshold = 1000 / cur->slotsInSlab;
		percentFull = 1000 * cur->slotFull / cur->slotNum;
		if(percentFull >= threshold) continue;
		kmem_cache_shrink(cur);
	}
}