#ifndef OS1_PROJEKAT_SYSCALL_HANDLERS_H
#define OS1_PROJEKAT_SYSCALL_HANDLERS_H

#include "../h/kernellib.h"
#include "../h/MemoryAllocator.h"
#include "tcb.h"

void __mem_alloc();
void __mem_free();

#endif //OS1_PROJEKAT_SYSCALL_HANDLERS_H
