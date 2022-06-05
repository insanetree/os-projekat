#include "../h/syscall_handlers.h"
#include "../h/semaphore.h"

void __sem_close_handler() {
	struct __semaphore* sem;
	int ret = 0;
	__asm__ volatile("mv %0, a1":"=r"(sem));
	if(sem == NULL) {
		ret = -0x22;
		__asm__ volatile("mv a0, %0"::"r"(ret));
		return;
	}
	__sem_close(sem);
	__asm__ volatile("mv a0, %0"::"r"(ret));
}