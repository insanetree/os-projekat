#include "../h/sleeper.h"
#include "../h/MemoryAllocator.h"
#include "../h/kernellib.h"

struct __tcb* sleep_list = NULL;

int __sleep_push(time_t sleep_time) {
	running->time = sleep_time;
	running->state = BLOCKED;

	struct __tcb* cur = sleep_list;
	struct  __tcb* prev = NULL;
	if(cur == NULL) {
		sleep_list = running;
		running->next = NULL;
		return 0;
	}

	while(cur != NULL && cur->time < sleep_time) {
		prev = cur;
		cur = cur->next;
	}

	if(prev != NULL)
		prev->next = running;
	else
		sleep_list = running;
	running->next = cur;
	return 0;
}

void __sleep_pop() {
	struct __tcb* cur = sleep_list;
	struct __tcb* thread;
	while(cur) {
		cur->time--;
		if(cur->time <= 0) {
			thread = cur;
			thread->state = READY;
			sleep_list = cur->next;
			__scheduler_push(thread);
		}
		cur = cur->next;
	}
}
