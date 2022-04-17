#include "../lib/console.h"
#include "../lib/hw.h"

//my header files
#include "../h/print.h"

void main(){
	printstr("hello\n");
	uint64 start = (uint64)HEAP_START_ADDR;
	uint64 end = (uint64)HEAP_END_ADDR;
	printunum((end-start)/MEM_BLOCK_SIZE);
	printstr("\n");
	int* t = (int*)HEAP_START_ADDR;
	*t = 10;
	printunum(*t);
}
