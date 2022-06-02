#include "../h/tcb.h"
#include "../h/MemoryAllocator.h"

void some_jump() {
	__asm__ volatile("csrw sepc, ra");
	__asm__ volatile("sret");
}

void __thread_wrapper() {
	running->body(running->arg);
	running->finished = YES;
}

struct __tcb* __thread_create(Body body, void* arg) {
	struct __tcb* newThread = __MA_allocate(sizeof(struct __tcb));

	if(!newThread) return NULL;

	if(!body) {
		newThread->stack = NULL;
		newThread->sp = 0;
	} else {
		newThread->stack = __MA_allocate(DEFAULT_STACK_SIZE);
		newThread->sp = (uint64)(newThread->stack + DEFAULT_STACK_SIZE);
	}

	newThread->arg = arg;
	newThread->body = body;
	newThread->ra = (uint64) &__thread_wrapper;
	newThread->finished = NO;

	//__scheduler_push(newThread);

	return newThread;
}

void __thread_dispatch() {
	struct __tcb* old = running;
	if(old->finished == NO)
		__scheduler_push(old);
	running = __scheduler_pop();
	__switch_context(old, running);
}
