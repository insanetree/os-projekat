#include "../lib/console.h"
#include "../lib/hw.h"

//my header files
#include "../h/print.h"
#include "../h/kernellib.h"



void main() {
	__init_system();
	asm volatile("ecall");
	while(1){
		int i = 0;
		i++;
	}
}
