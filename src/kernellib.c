#include "../h/kernellib.h"
#include "../h/MemoryAllocator.h"
#include "../lib/console.h" //DOK NE ZAVRSIM ULAZ I IZLAZ

uint64 __align(uint64 what, uint64 to);
void __interrupt();
void __interrupt_handler();

uint64 __align(uint64 what, uint64 to) {
	return what % to ? what + (to - what % to) : what;
}

void __init_system() {
	//memory initialization
	__MA_memory_init();

	//set interrupt_handler address in stvec
	__asm__ volatile("csrw stvec, %0 ": : "r" (&__interrupt));

	//go to user mode

	//enable interrupt ALWAYS AT THE END
	__asm__ volatile("csrs sstatus, 0x02");
}

void __interrupt_handler(){
	uint64 scause;
	__asm__ volatile("csrr %0, scause":"=r"(scause));
	switch(scause) {
		case ((1ul << 63) | 1):
			__putc('t');
			__putc('\n');
			break;
		case 8:
		case 9:
			__putc('e');
			__putc('\n');
			break;
	}
	console_handler();
}