#ifndef OS1_PROJEKAT_SYSCALL_C_H
#define OS1_PROJEKAT_SYSCALL_C_H

#include "../h/kernellib.h"

void* mem_alloc(size_t size);
int mem_free(void* ptr);

#endif //OS1_PROJEKAT_SYSCALL_C_H
