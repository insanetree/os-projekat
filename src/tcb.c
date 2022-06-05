#include "../h/tcb.h"
#include "../h/MemoryAllocator.h"
#include "../h/syscall_c.h"

struct __node* exited = NULL;

void some_jump() {
	//uint64 spie = 0x20;// spp = 0x80;
	__asm__ volatile("csrw sepc, ra");
	//__asm__ volatile("csrs sstatus, %0"::"r"(spie));
	//__asm__ volatile("csrc sstatus, %0"::"r"(spp));
	__asm__ volatile("sret");
}

void __thread_wrapper() {
	some_jump();
	running->body(running->arg);
	thread_exit();
}

struct __tcb* __thread_create(Body body, void* arg) {
	struct __tcb* newThread = __MA_allocate(sizeof(struct __tcb));

	if(!newThread) return NULL;

	if(!body) {
		newThread->stack = NULL;
		newThread->sp = 0;
	} else {
		newThread->stack = __MA_allocate(2*DEFAULT_STACK_SIZE);
		newThread->sp = (uint64)(newThread->stack + DEFAULT_STACK_SIZE);
	}

	newThread->arg = arg;
	newThread->body = body;
	newThread->ra = (uint64) &__thread_wrapper;
	newThread->time = 0;
	newThread->state = READY;

	//__scheduler_push(newThread);

	return newThread;
}

void __thread_dispatch() {
	struct __tcb* old = running;
	if(old->state == READY)
		__scheduler_push(old);
	running = __scheduler_pop();
	__switch_context(old, running);
}

void __thread_exit() {
	running->state = FINISHED;
	__push_exit_stack(running);
	__thread_dispatch();
}

void __push_exit_stack(struct __tcb* thread) {
	struct __node* newNode = __MA_allocate(sizeof(struct __node));
	newNode->next = exited;
	newNode->d = thread;
	exited = newNode;
}

void __clear_exit_stack() {
	struct __node* cur = exited;
	struct __node* prev;
	while(cur != NULL) {
		prev = cur;
		cur = cur->next;
		__MA_free(((struct __tcb*)(prev->d))->stack);
		__MA_free(prev->d);
		__MA_free(prev);
	}
	exited = NULL;
}