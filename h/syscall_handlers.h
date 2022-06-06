#ifndef OS1_PROJEKAT_SYSCALL_HANDLERS_H
#define OS1_PROJEKAT_SYSCALL_HANDLERS_H

#include "../h/kernellib.h"
#include "../h/MemoryAllocator.h"
#include "tcb.h"

void __mem_alloc_handler();
void __mem_free_handler();
void __thread_exit_handler();
void __thread_create_handler();
void __sem_open_handler();
void __sem_close_handler();
void __sem_wait_handler();

#endif //OS1_PROJEKAT_SYSCALL_HANDLERS_H
