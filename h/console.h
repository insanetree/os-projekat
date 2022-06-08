#ifndef OS1_PROJEKAT_CONSOLE_H
#define OS1_PROJEKAT_CONSOLE_H

#define BUFFER_SIZE 64

#include "../lib/hw.h"

#ifdef __cplusplus
extern "C" {
#endif

void __init_console();

void console_handler();

void output(char c);

char input();

#ifdef __cplusplus
}
#endif

#endif //OS1_PROJEKAT_CONSOLE_H
