#include "../h/syscall_handlers.h"
#include "../h/sleeper.h"

void __time_sleep_handler() {
	int ret = 0;
	time_t time;
	time_t runtime = running->time;
	__asm__ volatile("mv %0, a1":"=r"(time));
	if(__sleep_push(time) != 0) {
		ret = -0x31;
		__asm__ volatile("mv a0, %0"::"r"(ret));
		return;
	}
	__thread_dispatch();
	running->time = runtime;
	__asm__ volatile("mv a0, %0"::"r"(ret));
}