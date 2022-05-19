#include "../lib/console.h"
#include "../lib/hw.h"

//my header files
#include "../h/print.h"
#include "../h/MemoryAllocator.h"

void main() {
	__MA_memory_init();
	printunum((uint64)FREE_SPACE_START);
	uint8 * ptr1 = __MA_allocate(4*MEM_BLOCK_SIZE);
	uint8 * ptr2 = __MA_allocate(4*MEM_BLOCK_SIZE);
	uint8 * ptr3 = __MA_allocate(4*MEM_BLOCK_SIZE);
	uint8 * ptr4 = __MA_allocate(4*MEM_BLOCK_SIZE);
	__MA_free(ptr2); //da li se pomera FREE_SPACE_START? DA
	ptr2 = __MA_allocate(8*MEM_BLOCK_SIZE); //da li se alocira dobro? DA
	__MA_free(ptr1); //da li se spaja prostor?
	ptr1 = __MA_allocate(8*MEM_BLOCK_SIZE);
	ptr3[0] = 0xff;
	ptr4[0] = 0xff;
}
