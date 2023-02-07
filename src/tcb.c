#include "../h/tcb.h"
#include "../h/MemoryAllocator.h"
#include "../h/syscall_c.h"
#include "../h/cache.h"

struct __tcb* exited = NULL;

void some_jump() {
	//uint64 spie = 0x20;//
	//uint64 spp = 0x100;
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
	struct __tcb* newThread = kmem_cache_alloc(tcb_cache);

	if(!newThread) return NULL;

	if(!body) {
		newThread->stack = NULL;
		newThread->sp = 0;
		newThread->state = READY;
	} else {
		newThread->stack = kmem_cache_alloc(stack_cache);
		if(newThread->stack == NULL) {
			kmem_cache_free(tcb_cache, newThread);
			return NULL;
		}
		newThread->sp = (uint64)(newThread->stack + DEFAULT_STACK_SIZE);
		newThread->state = READY;
	}

	newThread->arg = arg;
	newThread->body = body;
	newThread->ra = (uint64) &__thread_wrapper;
	newThread->time = 0;
	newThread->next = NULL;
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
	thread->next = exited;
	exited = thread;
}

void __clear_exit_stack() {
	struct __tcb* cur = exited;
	struct __tcb* prev;
	while(cur != NULL) {
		prev = cur;
		cur = cur->next;
		kmem_cache_free(stack_cache,prev->stack);
		kmem_cache_free(tcb_cache, prev);
	}
	exited = NULL;
}