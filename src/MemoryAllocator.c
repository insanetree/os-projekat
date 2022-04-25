#include "../h/MemoryAllocator.h"
#include "../h/usrlib.h"

void* RESERVED_END_ADDR; 
void* FREE_SPACE_START;

struct __MA_memory_block {
	size_t size;
	struct __MA_memory_block* next;
};

/**
 * This function reserves space for monitoring allocated memory
 * After this RESERVED_END_ADDR will become a start addr for heap memory
 * Function MUST be run first to initialize memory
 */
void __MA_reserve_space();
void __MA_memory_init();
/**
* This function is for allocating memory
* allocates enough blocks that size fits in them
* returns a pointer to a allocated blocks
* updates reserved space
* updates FREE_SPACE_START
*/
void* __MA_allocate(size_t);
/**
* Marks all blocks from ptr in reserved space as used
* So one logical entity can be identified
*/
void __MA_mark_blocks(void*, size_t);

void __MA_reserve_space() {
	uint64 all_bytes = HEAP_END_ADDR - HEAP_START_ADDR;
	uint64 res_bytes;
	const uint8 two_bit_sections = 4;
	uint64 div = two_bit_sections*MEM_BLOCK_SIZE + 1;

	{//reserve the least amount of bytes needed to track all blocks
		uint64 tmp;
		tmp = __allign(all_bytes, div);
		res_bytes = tmp / div;
	}
	
	RESERVED_END_ADDR = (void*)HEAP_START_ADDR + res_bytes;
	
	//align reserved space to a block
	RESERVED_END_ADDR = (void*)__allign((uint64)RESERVED_END_ADDR, MEM_BLOCK_SIZE);
}

void __MA_memory_init() {
	size_t block_size;
	struct __MA_memory_block* start;
	__MA_reserve_space();
	FREE_SPACE_START = RESERVED_END_ADDR;
	start = (struct __MA_memory_block*)FREE_SPACE_START;
	block_size = ((uint64)HEAP_END_ADDR - (uint64)FREE_SPACE_START);
	start->size = block_size;
	start->next = NULL;
}


void* __MA_allocate(size_t size) {
	struct __MA_memory_block* head = FREE_SPACE_START;
	struct __MA_memory_block* i = head;
	struct __MA_memory_block* pri = NULL;
	struct __MA_memory_block* bstft = NULL;
	struct __MA_memory_block* prbstft;
	uint64 reqspc = __allign(size, MEM_BLOCK_SIZE);

	//if no free space
	if(FREE_SPACE_START == NULL)
		return NULL;

	do {
		if(i->size >= reqspc && (!bstft || bstft->size < i->size)) {
			prbstft = pri;
			bstft = i;
		}
		pri = i;
		i = i->next;
	}while(i != NULL);

	//if none is found
	if(bstft == NULL){
		return NULL;
	}

	if(bstft->size > reqspc) {
		struct __MA_memory_block* tmp = NULL;
		tmp = (void*)bstft + reqspc;
		tmp->next = bstft->next;
		tmp->size = bstft->size - reqspc;
		bstft->next = tmp;
	}

	if(bstft == FREE_SPACE_START) {
		FREE_SPACE_START = bstft->next;
	}else {
		prbstft->next = bstft->next;
	}

	__MA_mark_blocks((void*)bstft, reqspc);

	return (void*)bstft;
}

void __MA_mark_blocks(void* ptr, size_t size) {
	static const uint8 masks[] = {0x03, 0x0c, 0x30, 0xc0};
	static const uint8 flags[] = {0x00, 0x55, 0xaa, 0xff};
	uint8* base = (uint8*)HEAP_START_ADDR;
	uint64 blocks = size / MEM_BLOCK_SIZE;
	uint64 byte = (uint64)(ptr - RESERVED_END_ADDR)/(MEM_BLOCK_SIZE*CRUMBS);
	uint8 crumb = ((uint64)(ptr - RESERVED_END_ADDR)/(MEM_BLOCK_SIZE)) % CRUMBS;
	uint8 flg = 3;

	{

	}

	while(blocks > 0){
		base[byte] &= ~masks[crumb] & NULL;
		base[byte] |= masks[crumb]&flags[flg];
		crumb++;
		crumb%=CRUMBS;
		if(crumb==0) byte++;
		blocks--;
	}
}