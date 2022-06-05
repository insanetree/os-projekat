#ifndef OS1_PROJEKAT_SEMAPHORE_H
#define OS1_PROJEKAT_SEMAPHORE_H

#include "kernellib.h"
#include "tcb.h"
#include "scheduler.h"

struct __semaphore {
	long value;
	struct __list* threads;
};

struct __semaphore* __sem_init(uint64 init);

void __push_sem(struct __semaphore* sem, struct __tcb* thread);

void __pop_sem(struct __semaphore* sem, enum thread_state state);

void __sem_close(struct __semaphore* sem);

#endif //OS1_PROJEKAT_SEMAPHORE_H
