#include "../h/scheduler.h"
#include "../h/kernellib.h"
#include "../h/MemoryAllocator.h"
#include "../h/tcb.h"

struct __tcb* running = NULL;

struct __tcb* head;
struct __tcb* tail;

void __init_scheduler() {
	head = NULL;
	tail = NULL;
	struct __tcb* kernel = __thread_create(NULL, NULL);
	running = kernel;
}

void __scheduler_push(struct __tcb* newThread) {
	newThread->next = NULL;
	if(!tail) {
		tail = newThread;
		head = newThread;
	} else {
		tail->next = newThread;
		tail = newThread;
	}
}
struct __tcb* __scheduler_pop() {
	struct __tcb* front = head;
	if(!front) return NULL;
	head = head->next;
	if(head == NULL)
		tail = NULL;
	return front;
}
