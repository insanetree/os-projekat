#ifndef OS1_PROJEKAT_TCB_H
#define OS1_PROJEKAT_TCB_H

#include "kernellib.h"
#include "scheduler.h"

extern struct __node* exited;

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
	uint64 time;
	enum thread_finished finished;
};

void __thread_wrapper();

struct __tcb* __thread_create(Body, void*);

void __push_exit_stack(struct __tcb*);

/**
 * only supposed to be run by kernel thread (body == NULL)
 */
void __clear_exit_stack();

void __thread_exit();

void __thread_dispatch();

void __switch_context(struct __tcb*, struct __tcb*);

#endif //OS1_PROJEKAT_TCB_H
