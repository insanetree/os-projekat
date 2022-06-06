#include "../h/syscall_handlers.h"
#include "../h/semaphore.h"

void __sem_wait_handler() {
	int ret = 0;
	struct __semaphore* sem;
	__asm__ volatile("mv %0, a1":"=r"(sem));
	if(sem == NULL) {
		ret = -0x23;
		__asm__ volatile("mv a0, %0"::"r"(ret));
		return;
	}
	ret = __sem_wait(sem);
	__asm__ volatile("mv a0, %0"::"r"(ret));
}