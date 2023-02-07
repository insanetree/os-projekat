#ifndef OS1_PROJEKAT_TCB_H
#define OS1_PROJEKAT_TCB_H

#include "kernellib.h"
#include "scheduler.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef void(*Body)(void*);

enum thread_state {
	FINISHED,
	READY,
	BLOCKED,
	IRREGULAR_POST,
};

struct __tcb {
	uint64 ra;
	uint64 sp;
	void* stack;
	Body body;
	void* arg;
	long time;
	enum thread_state state;
	struct __tcb* next;
};

extern struct __tcb* exited;

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

#ifdef __cplusplus
}
#endif


#endif //OS1_PROJEKAT_TCB_H
