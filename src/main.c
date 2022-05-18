#include "../lib/console.h"
#include "../lib/hw.h"

//my header files
#include "../h/print.h"
#include "../h/MemoryAllocator.h"

void main() {
	__MA_memory_init();
	printunum((uint64)FREE_SPACE_START);
	uint8 * ptr1 = __MA_allocate(5*MEM_BLOCK_SIZE);
	uint8 * ptr2 = __MA_allocate(5*MEM_BLOCK_SIZE);
	uint8 * ptr3 = __MA_allocate(100000000*MEM_BLOCK_SIZE);
	ptr1[0]=0xff;
	ptr2[0]=0xff;
	ptr3[0]=0xff;
}
