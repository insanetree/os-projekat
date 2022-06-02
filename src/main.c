#include "../lib/hw.h"

//my header files
#include "../h/kernellib.h"
#include "../h/syscall_c.h"
#include "../h/tcb.h"

int aa = 0;

void a(void* ar){
	while(1) {
		aa++;
		__thread_dispatch();
	}
}

int bb = 0;

void b(void* ar){
	while(1) {
		bb++;
		__thread_dispatch();
	}
}

int mm = 0;

int main() {
	__init_system();
	thread_t ta;
	thread_t tb;

	int ret = thread_create(&ta, a, NULL);

	if(ret != 0){
		return -1;
	}

	ret = thread_create(&tb, b, NULL);

	if(ret != 0){
		return -1;
	}

	while(aa < 10000 || bb < 10000){
		mm++;
		thread_dispatch();
	}


	return 0;
}
