#include "../h/MemoryAllocator.h"
#include "../h/kernellib.h"

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

uint8 __MA_get_crumb(uint64 byte, uint8 crumb);

uint8 __MA_get_flag(uint8 f1, uint8 f2);

uint64 __MA_get_free_size(void* ptr);

void __MA_try_to_join(struct __MA_memory_block* b, struct __MA_memory_block* a);

void __MA_reserve_space() {
	uint64 all_bytes = HEAP_END_ADDR - HEAP_START_ADDR;
	uint64 res_bytes;
	uint64 div = CRUMBS*MEM_BLOCK_SIZE + 1;

	{//reserve the least amount of bytes needed to track all blocks
		uint64 tmp;
		tmp = __align(all_bytes, div);
		res_bytes = tmp / div;
	}
	
	RESERVED_END_ADDR = (void*)HEAP_START_ADDR + res_bytes;
	
	//align reserved space to a block
	RESERVED_END_ADDR = (void*) __align((uint64) RESERVED_END_ADDR, MEM_BLOCK_SIZE);
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
	uint64 reqspc = __align(size, MEM_BLOCK_SIZE);

	//if no free space
	if(FREE_SPACE_START == NULL)
		return NULL;

	do {
		if(i->size >= reqspc && (!bstft || bstft->size > i->size)) {
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
	uint8 flg = 3, flgBefore, flgAfter;

	{
		uint8 crumbBefore = (crumb - 1);
		crumbBefore %= CRUMBS;
		uint64 byteBefore = byte;
		if(crumbBefore == 3){
			if(byte)byteBefore--;
			else crumbBefore=0;
		}
		flgBefore = __MA_get_crumb(byteBefore, crumbBefore);

		uint8 crumbAfter = (crumb + blocks)%CRUMBS;
		uint64 byteAfter = byte + (crumb+blocks)/CRUMBS;
		flgAfter = __MA_get_crumb(byteAfter, crumbAfter);
	}

	flg = __MA_get_flag(flgBefore, flgAfter);

	while(blocks > 0){
		base[byte] &= ~masks[crumb];
		base[byte] |= masks[crumb]&flags[flg];
		crumb++;
		crumb%=CRUMBS;
		if(crumb==0) byte++;
		blocks--;
	}
}

uint8 __MA_get_crumb(uint64 byte, uint8 crumb) {
    static const uint8 masks[] = {0x03, 0x0c, 0x30, 0xc0};
    uint8* base = (uint8*)HEAP_START_ADDR;
    return (base[byte]&masks[crumb]) >> (crumb*2);
}

uint8 __MA_get_flag(uint8 f1, uint8 f2) {
	uint8 map[4]={0, 0, 0, 0};
	map[f1]++;
	map[f2]++;
	for(int i = 0 ; i < 4 ; i++){
		if(map[i]==0) return i;
	}
	return 0; //never gets here
}

uint64 __MA_get_free_size(void* ptr) {
	uint64 byte = (uint64)(ptr - RESERVED_END_ADDR)/(MEM_BLOCK_SIZE*CRUMBS);
	uint8 crumb = ((uint64)(ptr - RESERVED_END_ADDR)/(MEM_BLOCK_SIZE)) % CRUMBS;
	uint64 size=0;
	uint8 flg = __MA_get_crumb(byte, crumb);
	while(flg == __MA_get_crumb(byte, crumb++)){
		size++;
		if(crumb == 4){
			crumb = 0;
			byte++;
		}
	}
	return size*MEM_BLOCK_SIZE;
}

void __MA_free(void* ptr){
	size_t freeSize = __MA_get_free_size(ptr);
	struct __MA_memory_block* newBlock = (struct __MA_memory_block*)ptr;
	newBlock->size = freeSize;
	newBlock->next = NULL;
	struct __MA_memory_block* cur = FREE_SPACE_START;
	struct __MA_memory_block* prev = NULL;
	if(ptr < (void*)cur || cur == NULL) {
		FREE_SPACE_START = (void*)newBlock;
		newBlock->next = cur;
	}
	else {
		while(cur != NULL && (void*)cur < ptr) {
			prev = cur;
			cur = cur->next;
		}
		prev->next = newBlock;
		newBlock->next = cur;
	}
	__MA_try_to_join(newBlock, cur);
	__MA_try_to_join(prev, newBlock);
}

void __MA_try_to_join(struct __MA_memory_block* b, struct __MA_memory_block* a){
	if(a == NULL || b == NULL) return;
	if((void*)b + b->size == (void*)a) {
		b->size += a->size;
		b->next = a->next;
	}
}

/*
 Ja dok krsim unix filozofiju:
⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠛⠛⠋⠉⠈⠉⠉⠉⠉⠛⠻⢿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⡿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⢿⣿⣿⣿⣿
⣿⣿⣿⣿⡏⣀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿
⣿⣿⣿⢏⣴⣿⣷⠀⠀⠀⠀⠀⢾⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿
⣿⣿⣟⣾⣿⡟⠁⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣷⢢⠀⠀⠀⠀⠀⠀⠀⢸⣿
⣿⣿⣿⣿⣟⠀⡴⠄⠀⠀⠀⠀⠀⠀⠙⠻⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⣿
⣿⣿⣿⠟⠻⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠶⢴⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⣿
⣿⣁⡀⠀⠀⢰⢠⣦⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⡄⠀⣴⣶⣿⡄⣿
⣿⡋⠀⠀⠀⠎⢸⣿⡆⠀⠀⠀⠀⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⠗⢘⣿⣟⠛⠿⣼
⣿⣿⠋⢀⡌⢰⣿⡿⢿⡀⠀⠀⠀⠀⠀⠙⠿⣿⣿⣿⣿⣿⡇⠀⢸⣿⣿⣧⢀⣼
⣿⣿⣷⢻⠄⠘⠛⠋⠛⠃⠀⠀⠀⠀⠀⢿⣧⠈⠉⠙⠛⠋⠀⠀⠀⣿⣿⣿⣿⣿
⣿⣿⣧⠀⠈⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠟⠀⠀⠀⠀⢀⢃⠀⠀⢸⣿⣿⣿⣿
⣿⣿⡿⠀⠴⢗⣠⣤⣴⡶⠶⠖⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡸⠀⣿⣿⣿⣿
⣿⣿⣿⡀⢠⣾⣿⠏⠀⠠⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠉⠀⣿⣿⣿⣿
⣿⣿⣿⣧⠈⢹⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿
⣿⣿⣿⣿⡄⠈⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣾⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣦⣄⣀⣀⣀⣀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡄⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠙⣿⣿⡟⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠁⠀⠀⠹⣿⠃⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⢐⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⠿⠛⠉⠉⠁⠀⢻⣿⡇⠀⠀⠀⠀⠀⠀⢀⠈⣿⣿⡿⠉⠛⠛⠛⠉⠉
⣿⡿⠋⠁⠀⠀⢀⣀⣠⡴⣸⣿⣇⡄⠀⠀⠀⠀⢀⡿⠄⠙⠛⠀⣀⣠⣤⣤⠄⠀
 */