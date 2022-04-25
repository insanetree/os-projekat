#include "../lib/console.h"
#include "../lib/hw.h"

//my header files
#include "../h/print.h"
#include "../h/MemoryAllocator.h"

void main() {
	__MA_memory_init();
	printunum((uint64)FREE_SPACE_START);
	uint8 * ptr = __MA_allocate(5*MEM_BLOCK_SIZE);
	ptr[0]=0xff;
}
