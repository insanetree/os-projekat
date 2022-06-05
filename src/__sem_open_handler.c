#include "../h/semaphore.h"
#include "../h/syscall_handlers.h"

void __sem_open_handler() {
	struct __semaphore** handle;
	long init;
	int ret = 0;
	__asm__ volatile("mv %0, a1":"=r"(handle));
	__asm__ volatile("mv %0, a2":"=r"(init));
	*handle = __sem_init(init);
	if(*handle == NULL) {
		ret = -0x21;
		__asm__ volatile("mv a0, %0"::"r"(ret));
		return;
	}
	__asm__ volatile("mv a0, %0"::"r"(ret));
}