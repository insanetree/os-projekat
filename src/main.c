#include "../lib/hw.h"

//my header files
#include "../h/kernellib.h"
#include "../h/syscall_c.h"
#include "../lib/console.h"

unsigned aa = 0;
int as = 0;
int ae = 0;

void a(void* ar){
	while(as == 0){
		aa++;
		//__putc('a');
		//__putc('\n');
	}
	ae++;
	//thread_exit();
}

unsigned bb = 0;
int bs = 0;
int be = 0;

void b(void* ar){
	while(bs == 0){
		bb++;
		//__putc('b');
		//__putc('\n');
	}
	be++;
	//thread_exit();
}

int mm = 0;

int main() {
	__init_system();
	thread_t ta = NULL;
	thread_t tb = NULL;

	sem_t sem = NULL;

	int ret = thread_create(&ta, a, NULL);

	if(ret != 0){
		return -1;
	}

	ret = thread_create(&tb, b, NULL);

	if(ret != 0){
		return -1;
	}


	while(aa < 100 || bb < 100){
		mm++;
		thread_dispatch();
	}

	as = 1;
	bs = 1;

	while(ae == 0 || be == 0);

	ret = sem_open(&sem, 1);

	if(ret != 0){
		return -1;
	}

	return 0;
}
