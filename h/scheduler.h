#ifndef OS1_PROJEKAT_SCHEDULER_H
#define OS1_PROJEKAT_SCHEDULER_H

#include "tcb.h"

#ifdef __cplusplus
extern "C" {
#endif
extern struct __tcb* running;

void __init_scheduler();

void __scheduler_push(struct __tcb*);
struct __tcb* __scheduler_pop();

#ifdef __cplusplus
}
#endif


#endif //OS1_PROJEKAT_SCHEDULER_H
