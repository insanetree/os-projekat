#include "../h/syscall_handlers.h"

void __thread_start_handler() {
	int ret;
	struct __tcb* handle;
	__asm__ volatile("mv %0, a1":"=r"(handle));
	if(handle->state == CREATED) {
		ret = 0;
		__scheduler_push(handle);
	}
	else {
		ret = -0x14;
	}
	__asm__ volatile("mv a0, %0"::"r"(ret));
}
