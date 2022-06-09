#include "../h/syscall_c.h"
#include "../lib/console.h"

char getc() {
	return __getc();
}

void putc(char c) {
	__putc(c);
}