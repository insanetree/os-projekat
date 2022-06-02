#include "../h/syscall_handlers.h"
#include "../h/tcb.h"
#include "../h/kernellib.h"

void __thread_create_handler() {
	Body body;
	void* arg;
	struct __tcb** handle;
	uint64 ret;
	__asm__ volatile("mv %0, a1":"=r"(handle));
	__asm__ volatile("mv %0, a2":"=r"(body));
	__asm__ volatile("mv %0, a3":"=r"(arg));

	*handle = __thread_create(body, arg);

	if(*handle)
		ret = 0;
	else
		ret = -0x11;

	__asm__ volatile("mv a0, %0"::"r"(ret));
}