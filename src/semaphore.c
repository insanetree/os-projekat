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

void __pop_sem(struct __semaphore* sem) {
	struct __tcb* thread = __pop_front(sem->threads);
	thread->state = READY;
	__scheduler_push(thread);
}