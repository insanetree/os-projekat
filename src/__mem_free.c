#include "../h/syscall_handlers.h"

void __mem_free(){
	void* ptr;
	int ret;
	__asm__ volatile("mv %0, a1":"=r"(ptr));
	ret = __MA_free(ptr);
	__asm__ volatile("mv a0, %0"::"r"(ret));
}