#include "../lib/hw.h"

//my header files
#include "../h/kernellib.h"
#include "../h/syscall_c.h"
#include "../lib/console.h"

thread_t ta = NULL;
thread_t tb = NULL;

sem_t sem = NULL;

unsigned aa = 0;
int ae = 0;

void a(void* ar){
	int ret;
	ret = sem_wait(sem);
	if(ret != 0) {
		ae++;
	}
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
 	int ret;
	ret = sem_open(&sem, 0);

	if(ret != 0){
		return -1;
	}

	ret = thread_create(&ta, a, NULL);

	if(ret != 0){
		return -1;
	}

	ret = thread_create(&tb, b, NULL);

	if(ret != 0){
		return -1;
	}


	while(bb < 100){
		mm++;
		thread_dispatch();
	}

	bs = 1;


	ret = sem_close(sem);

	if(ret != 0){
		return -1;
	}

	while(ae == 0 || be == 0);




	return 0;
}
