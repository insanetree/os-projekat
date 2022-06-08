#include "../h/console.h"
#include "../h/semaphore.h"

/*
 * everything is happening inside of kernel call
 * no interrupt is possible during these functions
 */

char* tx;
char* rx;
char* status;

char input_buffer[BUFFER_SIZE];
uint8 input_head;
uint8 input_tail;
struct __semaphore* iput;
struct __semaphore* itake;

char output_buffer[BUFFER_SIZE];
uint8 output_head;
uint8 output_tail;
struct __semaphore* oput;
struct __semaphore* otake;

void console_handler() {
	int irq = plic_claim();
	if(irq != CONSOLE_IRQ)
		return;

	while(*status & CONSOLE_TX_STATUS_BIT && output_head != output_tail) {
		__sem_wait(otake);
		*rx = output_buffer[output_head];
		output_head = (output_head+1)%BUFFER_SIZE;
		__sem_signal(oput);
	}

	plic_complete(irq);
}

void __init_console() {
	iput = __sem_init(BUFFER_SIZE);
	itake = __sem_init(0);
	oput = __sem_init(BUFFER_SIZE);
	otake = __sem_init(0);
	tx = (char*)CONSOLE_TX_DATA;
	rx = (char*)CONSOLE_RX_DATA;
	status = (char*)CONSOLE_STATUS;
}

void output(char c) {
	__sem_wait(oput);
	output_buffer[output_tail++] = c;
	output_tail %= BUFFER_SIZE;
	__sem_signal(otake);
}