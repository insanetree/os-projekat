#include "../lib/hw.h"

//my header files
#include "../h/kernellib.h"
#include "../h/syscall_c.h"
#include "../lib/console.h"

int aa = 0;
int as = 0;

void a(void* ar){
	while(as == 0){
		aa++;
		__putc('a');
		__putc('\n');
	}
	thread_exit();
}

int bb = 0;
int bs = 0;

void b(void* ar){
	while(bs == 0){
		bb++;
		__putc('b');
		__putc('\n');
	}
	thread_exit();
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

	as = 1;
	bs = 1;

	while(1);

	return 0;
}
