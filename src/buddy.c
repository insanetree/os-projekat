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
#define minBuddySize 5
#define numOfBuckets 20

struct Buddy *buddy[numOfBuckets];

/**
 * private functions
 */

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
	return NULL;
}

void buddy_free(void *address) {

}

void *buddy_get(uint8 bucket) {
	struct Buddy *ret = buddy[bucket];
	if (buddy[bucket] == NULL) {
		ret = buddy_shrink(bucket);
		if (!ret)
			return NULL;
	}
	if (ret->next)
		ret->next->prev = NULL;
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
	while (startBucket > bucket) {
		startBucket--;
		right = (struct Buddy *) ((uint64) left + (1 << (startBucket + minBuddySize)));
		buddy_put(right, startBucket);
	}
	return left;
}

void buddy_put(struct Buddy *block, uint8 bucket) {
	struct Buddy *cur = buddy[bucket];
	if (!cur) {
		block->next = block->prev = NULL;
		buddy[bucket] = block;
		return;
	}
	while (cur < block && cur->next)
		cur = cur->next;
	if (block < buddy[bucket]) {
		cur->prev = block;
		block->next = cur;
		block->prev = NULL;
		buddy[bucket] = block;
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
	if ((addressLeft & addressRight) != 1 << bucket)
		return;
	if (block == buddy[bucket]) {
		buddy[bucket] = block->next->next;
		block->next->next->prev = NULL;
		return;
	}
	block->prev->next = block->next->next;
	if(block->next->next)
		block->next->next->prev = block->prev;
	buddy_put(block, bucket+1);
}