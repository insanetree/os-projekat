#ifndef OS1_PROJEKAT_SYSCALL_C_H
#define OS1_PROJEKAT_SYSCALL_C_H

//#include "kernellib.h"
#include "tcb.h"
#include "semaphore.h"

#ifdef __cplusplus
extern "C" {
#endif

void* mem_alloc(size_t size);

int mem_free(void* ptr);

typedef struct __tcb* thread_t;

int thread_create(thread_t* handle, void(*f)(void*), void* arg);

int thread_exit();

void thread_dispatch();

typedef struct __semaphore* sem_t;

int sem_open(sem_t* handle, uint64 init);

int sem_close(sem_t handle);

int sem_wait(sem_t handle);

int sem_signal(sem_t handle);

int time_sleep(time_t);

#ifdef __cplusplus
}
#endif


#endif //OS1_PROJEKAT_SYSCALL_C_H
