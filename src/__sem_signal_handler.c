#include "../h/syscall_handlers.h"
#include "../h/semaphore.h"

void __sem_signal_handler() {
	int ret = 0;
	struct __semaphore* sem;
	__asm__ volatile("mv %0, a1":"=r"(sem));
	if(sem == NULL) {
		ret = -0x24;
		__asm__ volatile("mv a0, %0"::"r"(ret));
		return;
	}
	__sem_signal(sem);
	__asm__ volatile("mv a0, %0"::"r"(ret));
}