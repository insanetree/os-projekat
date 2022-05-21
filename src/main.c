#include "../lib/console.h"
#include "../lib/hw.h"

//my header files
#include "../h/syscall_c.h"



void main() {
	__init_system();
	int* a = mem_alloc(0xab);
	a[0] = 2;
}
