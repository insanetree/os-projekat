#include "../lib/console.h"
#include "../lib/hw.h"

//my header files
#include "../h/print.h"
#include "../h/MemoryAllocator.h"

void main() {
	__MA_memory_init();
	printunum((uint64)FREE_SPACE_START);
}
