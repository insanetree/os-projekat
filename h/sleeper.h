#ifndef OS1_PROJEKAT_SLEEPER_H
#define OS1_PROJEKAT_SLEEPER_H

#include "tcb.h"

#ifdef __cplusplus
extern "C" {
#endif

struct __sleep_node {
	struct __tcb* thread;
	time_t sleep_time;
	struct __sleep_node* next;
};

int __sleep_push(time_t sleep_time);

void __sleep_pop();

#ifdef __cplusplus
}
#endif


#endif //OS1_PROJEKAT_SLEEPER_H
