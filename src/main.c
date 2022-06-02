#include "../lib/hw.h"

//my header files
#include "../h/kernellib.h"
#include "../h/syscall_c.h"
#include "../h/tcb.h"

int aa = 0;

void a(void* ar){
	aa++;
	__thread_dispatch();
}

int bb = 0;

void b(void* ar){
	bb++;
	__thread_dispatch();
}

int main() {
	__init_system();
	thread_t ta;

	int ret = thread_create(&ta, a, NULL);
	ret++;

	if(ret == 1){
		ret++;
	} else {
		return -1;
	}

	while(ta->finished == NO){
		__thread_dispatch();
	}


	return 0;
}
