#include "../lib/hw.h"

//my header files
#include "../h/kernellib.h"
#include "../h/syscall_c.h"
#include "../h/sleeper.h"

thread_t usr;

void userMain(void* );

int main() {
	__init_system();

	thread_create(&usr, &userMain, NULL);

	while(usr->state != FINISHED)
		thread_dispatch();

	return 0;
}
