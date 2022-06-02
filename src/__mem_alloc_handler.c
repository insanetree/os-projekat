#include "../h/syscall_handlers.h"
#include "../h/kernellib.h"
#include "../h/MemoryAllocator.h"

void __mem_alloc_handler() {
	size_t size;
	void* ptr;
	__asm__ volatile("mv %0, a1":"=r"(size));
	ptr = __MA_allocate(size);
	__asm__ volatile("mv a0, %0"::"r"(ptr));
}