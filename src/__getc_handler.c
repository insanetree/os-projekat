#include "../h/console.h"
#include "../h/syscall_handlers.h"

void __getc_handler() {
	uint64 ret;
	ret = input();
	__asm__ volatile("mv a0, %0"::"r"(ret));
}
