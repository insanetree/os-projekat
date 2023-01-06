#include "../h/buddy.h"

#define NULL 0ul

struct Buddy{
	uint8 size;
	uint64 address;
	struct Buddy* next;
	struct Buddy* prev;
};
/**
 * number of buddy sizes
 * from 2^5 B to 2^24 B
 */
#define maxBuddySize 24
#define minBuddySize 5
#define numOfBuckets 20

struct Buddy* buddy[numOfBuckets];

/**
 * private functions
 */
void* buddy_get(uint8 size);
void buddy_put();
void buddy_merge(struct Buddy*);
void buddy_split(struct Buddy*);

void buddy_init() {
	struct Buddy* first = (struct Buddy*) BUDDY_START_ADDR;
	first->address = (uint64)BUDDY_START_ADDR;
	first->size = maxBuddySize;
	first->next = NULL;
	first->prev = NULL;
	buddy[numOfBuckets - 1] = first;
}

void* buddy_allocate(size_t size) {
	return NULL;
}

void buddy_free(void* address) {

}