#include "../h/buddy.h"

#define NULL 0ul

struct Buddy {
	struct Buddy *next;
	struct Buddy *prev;
};
/**
 * number of buddy sizes
 * from 2^5 B to 2^24 B
 */
#define maxBuddySize 24
extern uint64 minBuddySize;
extern uint64 numOfBuckets;
extern uint64 buddyBlockNum;
extern uint64 buddyBlockFree;
struct Buddy *buddy[64];

/**
 * private functions
 */
/**
 * returns first power of 2 that is larger than size
 * @param size size of elemet to allocate
 * @return power or -1 if error
 */
int buddy_get_bucket(size_t size);
/**
 * get first free block from bucket of and updates the list
 * @param size size of buddy
 * @return addres of free block, NULL if empty
 */
void *buddy_get(uint8 size);

/**
 * puts free block in ordered list
 * @param block free segment to put
 */
void buddy_put(struct Buddy *block, uint8 size);

void buddy_try_merge(struct Buddy *block, uint8 bucket);

/**
 * shrinks first larger block to required size
 * @param sizeTo required size
 * @return pointer to the new block, NULL if none exists
 */
void *buddy_shrink(uint8 sizeTo);

void buddy_init() {
	struct Buddy *first = (struct Buddy *) BUDDY_START_ADDR;
	first->next = NULL;
	first->prev = NULL;
	buddy[numOfBuckets - 1] = first;
}

void *buddy_allocate(size_t size) {
	int bucket = buddy_get_bucket(size);
	if(bucket < 0)
		return NULL;
	void* ret = buddy_get(bucket);
	if(ret) buddyBlockFree -= 1<<bucket;
	return ret;
}

void buddy_free(void *address, size_t size) {
	uint8 bucket = buddy_get_bucket(size);
	struct Buddy* block = (struct Buddy*)address;
	buddyBlockFree += 1<<bucket;
	buddy_put(block, bucket);
}

void *buddy_get(uint8 bucket) {
	struct Buddy *ret = buddy[bucket];
	if (buddy[bucket] == NULL) {
		ret = buddy_shrink(bucket);
		return ret;
	}
	if (ret->next) {
		ret->next->prev = NULL;
	}
	buddy[bucket] = ret->next;
	return ret;
}

void *buddy_shrink(uint8 bucket) {
	uint8 startBucket = bucket + 1;
	while (startBucket < numOfBuckets && buddy[startBucket] == NULL)
		startBucket++;
	if (startBucket == numOfBuckets)
		return NULL;
	struct Buddy *left = buddy_get(startBucket);
	struct Buddy *right;
	do{
		startBucket--;
		right = (struct Buddy *) ((uint64)left + (1 << (startBucket + minBuddySize)));
		buddy_put(right, startBucket);
	}while (startBucket > bucket);
	return left;
}

void buddy_put(struct Buddy *block, uint8 bucket) {
	struct Buddy *cur = buddy[bucket];
	if (!cur) {
		block->next = block->prev = NULL;
		buddy[bucket] = block;
		return;
	}
	while (cur->next && cur->next < block)
		cur = cur->next;
	if (block < buddy[bucket]) {
		cur->prev = block;
		block->next = cur;
		block->prev = NULL;
		buddy[bucket] = block;
		cur = buddy[bucket];
	} else {
		block->prev = cur;
		block->next = cur->next;
		if (cur->next)
			cur->next->prev = block;
		cur->next = block;
	}
	buddy_try_merge(cur, bucket);
}

void buddy_try_merge(struct Buddy *block, uint8 bucket) {
	if (!block->next)
		return;
	uint64 addressLeft = (uint64) block - (uint64) BUDDY_START_ADDR;
	uint64 addressRight = (uint64) block->next - (uint64) BUDDY_START_ADDR;
	if (addressRight != addressLeft + (1<<(bucket - 1 + minBuddySize)))
		return;
	if (block == buddy[bucket]) {
		buddy[bucket] = block->next->next;
		if(block->next->next)
			block->next->next->prev = NULL;
	} else {
		block->prev->next = block->next->next;
	}
	if(block->next->next)
		block->next->next->prev = block->prev;
	buddy_put(block, bucket+1);
}

static uint64 sizes[] = {1<<5, 1<<6, 1<<7, 1<<8, 1<<9,
			 1<<10, 1<<11, 1<<12, 1<<13, 1<<14,
			 1<<15, 1<<16, 1<<17, 1<<18, 1<<19,
			 1<<20, 1<<21, 1<<22, 1<<23, 1<<24,
			 1<<25, 1<<26, 1<<27, 1<<28, 1<<29};
int buddy_get_bucket(size_t size) {
	uint8 i = 0;
	while(size > sizes[i] && i < numOfBuckets)
		i++;
	if(i == numOfBuckets)
		return -1;
	return i;
}