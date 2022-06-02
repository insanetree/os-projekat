#ifndef OS1_PROJEKAT_TCB_H
#define OS1_PROJEKAT_TCB_H

#include "kernellib.h"
#include "scheduler.h"

typedef void(*Body)(void*);

enum thread_finished {
	YES,
	NO
};

struct __tcb {
	uint64 ra;
	uint64 sp;
	void* stack;
	Body body;
	void* arg;
	enum thread_finished finished;
};

void __thread_wrapper();

struct __tcb* __thread_create(Body, void*);

void __thread_dispatch();

void __switch_context(struct __tcb*, struct __tcb*);

#endif //OS1_PROJEKAT_TCB_H
