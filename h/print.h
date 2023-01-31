//
// Created by insanetree on 31.1.23..
//

#ifndef OS1_PROJEKAT_PRINT_H
#define OS1_PROJEKAT_PRINT_H
#include "syscall_c.h"

#ifdef __cplusplus
extern "C" {
#endif
void printstr(const char* str);

void printunum(uint64 num);
#ifdef __cplusplus
}
#endif

#endif //OS1_PROJEKAT_PRINT_H
