#include "../h/scheduler.h"
#include "../h/tcb.h"
#include "../h/kernellib.h"

void __thread_exit_handler() {
	int ret = 0;
	if(running->body == NULL){
		ret = -0x12;
		__asm__ volatile("mv a0, %0"::"r"(ret));
		return;
	}
	__thread_exit();
}