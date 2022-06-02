//
// Created by insanetree on 6/1/22.
//

#ifndef OS1_PROJEKAT_TCB_H
#define OS1_PROJEKAT_TCB_H

typedef void(*Body)(void*);

enum thread_finished {
	YES,
	NO
};

struct __tcb {
	uint64 ra;
	uint64 sp;
	Body body;
	enum thread_finished finished;
};


#endif //OS1_PROJEKAT_TCB_H
