#include "../lib/hw.h"

//my header files
#include "../h/kernellib.h"
#include "../h/syscall_c.h"

int aa = 0;

void a(void* ar){
	while(1) {
		for(uint64 i = 0 ; i < 0x8000000000000009UL ; i++);
		aa++;
		//thread_dispatch();
	}
}

int bb = 0;

void b(void* ar){
	while(1) {
		for(uint64 i = 0 ; i < 0x8000000000000009UL ; i++);
		bb++;
		//thread_dispatch();
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

	while(1);

	while(aa < 100 || bb < 100){
		mm++;
		thread_dispatch();
	}


	return 0;
}
