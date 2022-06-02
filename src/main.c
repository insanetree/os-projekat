#include "../lib/hw.h"

//my header files
#include "../h/kernellib.h"
#include "../h/syscall_c.h"
//#include "../h/MemoryAllocator.h"

int main() {
	__init_system();
	int reta, retb;
	int* a = mem_alloc(0xfffffffffffffff);
	reta = mem_free(a);
	int* b = mem_alloc(0xaa);
	retb = mem_free(b);
	if(reta < 0 && retb == 0){
		return 0;
	}
	return -1;
}
