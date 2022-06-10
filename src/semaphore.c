#include "../h/semaphore.h"
#include "../h/MemoryAllocator.h"

struct __semaphore* __sem_init(uint64 init) {
	struct __semaphore* newSem;
	newSem = __MA_allocate(sizeof(struct __semaphore));
	if(newSem == NULL)
		return NULL;
	newSem->value = init;
	newSem->head = NULL;
	newSem->tail = NULL;
	return newSem;
}

void __push_sem(struct __semaphore* sem, struct __tcb* thread) {
	thread->state = BLOCKED;
	thread->next = NULL;
	if(!sem->tail) {
		sem->tail = thread;
		sem->head = thread;
	} else {
		sem->tail->next = thread;
		sem->tail = thread;
	}
}

void __pop_sem(struct __semaphore* sem, enum thread_state state) {
	struct __tcb* thread = sem->head;
	if(thread == NULL)
		return;
	sem->head = sem->head->next;
	if(sem->head == NULL)
		sem->tail = NULL;
	thread->state = state;
	__scheduler_push(thread);
}

void __sem_close(struct __semaphore* sem) {
	while(sem->head != NULL) {
		__pop_sem(sem, IRREGULAR_POST);
	}
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
	if(sem->value <= 0 && sem->head != NULL) {
		__pop_sem(sem, READY);
	}
}