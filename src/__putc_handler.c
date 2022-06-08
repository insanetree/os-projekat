#include "../h/syscall_handlers.h"
#include "../h/console.h"

void __putc_handler() {
	uint64 a1;
	char c;
	__asm__ volatile("mv %0, a1":"=r"(a1));
	c = (char)a1;
	output(c);
}