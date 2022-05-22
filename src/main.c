#include "../lib/hw.h"

//my header files
#include "../h/syscall_c.h"
#include "../h/MemoryAllocator.h"

int main() {
	__init_system();
	int* a = mem_alloc(0xfffffffffffffff);
	__MA_free(a);
	int* b = __MA_allocate(0xfffffffffffffff);
	if(a == b){
		return 0;
	}
	else {
		return -1;
	}
}
