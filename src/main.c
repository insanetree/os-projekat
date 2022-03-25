#include "../lib/console.h"
#include "../lib/hw.h"

//my header files
#include "../h/print.h"

void main(){
	printstr("hello\n");
	uint64 start = (uint64)HEAP_START_ADDR;
	uint64 end = (uint64)HEAP_END_ADDR;
    printunum(123);
    printstr("\n");
    printunum(start);
    printstr("\n");
    printunum(end);
    printstr("\n");
}
