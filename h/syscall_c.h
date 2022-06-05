#ifndef OS1_PROJEKAT_SYSCALL_C_H
#define OS1_PROJEKAT_SYSCALL_C_H

//#include "kernellib.h"
#include "tcb.h"

void* mem_alloc(size_t size);
int mem_free(void* ptr);

typedef struct __tcb* thread_t;

int thread_create(thread_t* handle, void(*f)(void*), void* arg);

int thread_exit();

void thread_dispatch();

#endif //OS1_PROJEKAT_SYSCALL_C_H
