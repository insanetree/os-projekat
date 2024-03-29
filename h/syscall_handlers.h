#ifndef OS1_PROJEKAT_SYSCALL_HANDLERS_H
#define OS1_PROJEKAT_SYSCALL_HANDLERS_H

#include "../h/kernellib.h"
#include "../h/MemoryAllocator.h"
#include "tcb.h"

#ifdef __cplusplus
extern "C" {
#endif

void __mem_alloc_handler();
void __mem_free_handler();
void __thread_exit_handler();
void __thread_create_handler();
void __thread_start_handler();
void __sem_open_handler();
void __sem_close_handler();
void __sem_wait_handler();
void __sem_signal_handler();
void __time_sleep_handler();
void __getc_handler();
void __putc_handler();

#ifdef __cplusplus
}
#endif


#endif //OS1_PROJEKAT_SYSCALL_HANDLERS_H
