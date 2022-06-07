#include "../h/sleeper.h"
#include "../h/MemoryAllocator.h"
#include "../h/kernellib.h"

struct __sleep_node* sleep_list = NULL;

int __sleep_push(time_t sleep_time) {
	struct __sleep_node* newNode = __MA_allocate(sizeof(struct __sleep_node));
	if(newNode == NULL)
		return -1;
	newNode->thread = running;
	newNode->sleep_time = sleep_time;
	running->state = BLOCKED;

	struct __sleep_node* cur = sleep_list;
	struct  __sleep_node* prev = NULL;
	if(cur == NULL) {
		sleep_list = newNode;
		newNode->next = NULL;
		return 0;
	}

	while(cur != NULL && cur->sleep_time < sleep_time) {
		prev = cur;
		cur = cur->next;
	}

	if(prev != NULL)
		prev->next = newNode;
	else
		sleep_list = newNode;
	newNode->next = cur;
	return 0;
}

void __sleep_pop() {
	struct __sleep_node* cur = sleep_list;
	struct __tcb* thread;
	while(cur) {
		cur->sleep_time--;
		if(cur->sleep_time <= 0) {
			thread = cur->thread;
			thread->state = READY;
			sleep_list = cur->next;
			__MA_free(cur);
			__scheduler_push(thread);
		}
		cur = cur->next;
	}
}
