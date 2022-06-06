#include "../h/semaphore.h"
#include "../h/MemoryAllocator.h"

struct __semaphore* __sem_init(uint64 init) {
	struct __semaphore* newSem;
	newSem = __MA_allocate(sizeof(struct __semaphore));
	if(newSem == NULL)
		return NULL;
	newSem->value = init;
	newSem->threads = __MA_allocate(sizeof(struct __list));
	newSem->threads->head = NULL;
	newSem->threads->tail = NULL;
	return newSem;
}

void __push_sem(struct __semaphore* sem, struct __tcb* thread) {
	thread->state = BLOCKED;
	__push_back(sem->threads, thread);
}

void __pop_sem(struct __semaphore* sem, enum thread_state state) {
	struct __tcb* thread = __pop_front(sem->threads);
	if(thread == NULL)
		return;
	thread->state = state;
	__scheduler_push(thread);
}

void __sem_close(struct __semaphore* sem) {
	while(sem->threads->head != NULL) {
		__pop_sem(sem, IRREGULAR_POST);
	}
	__MA_free(sem->threads);
	__MA_free(sem);
}

int __sem_wait(struct __semaphore* sem) {
	sem->value--;
	if(sem->value >= 0) {
		return 0;
	}
	__push_sem(sem,running);
	__thread_dispatch();
	if(running->state == IRREGULAR_POST) {
		running->state = READY;
		return -0x23;
	}
	return 0;
}

void __sem_signal(struct __semaphore* sem) {
	sem->value++;
	if(sem->value >= 0 && sem->threads->head != NULL) {
		sem->value--;
		__pop_sem(sem, READY);
	}
}