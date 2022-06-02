#include "../h/kernellib.h"
#include "../h/MemoryAllocator.h"
#include "../h/syscall_handlers.h"

void __handle_syscall();
void __interrupt();
void __interrupt_handler();

uint64 __align(uint64 what, uint64 to) {
	return what % to ? what + (to - what % to) : what;
}

inline void __init_system() {
	//memory initialization
	__MA_memory_init();

	//set interrupt_handler address in stvec
	__asm__ volatile("csrw stvec, %0 ": : "r" (&__interrupt));

	//enable interrupt ALWAYS AT THE END
	//__asm__ volatile("csrs sstatus, 0x02");
}

void __interrupt_handler() {
	register uint64 scause;
	__asm__ volatile("csrr %0, scause":"=r"(scause));
	switch(scause) {
		case ((1ul << 63) | 1):
			//timer
			break;
		case 8:
		case 9:
			//ecall
			__handle_syscall();
			__asm__ volatile("csrr t0, sepc");
			__asm__ volatile("addi t0, t0, 4");
			__asm__ volatile("csrw sepc, t0");
			__asm__ volatile("csrc sip, 0x02");
			__asm__ volatile("sd a0, 80(fp)");
			break;
	}
}

void __handle_syscall() {
	register uint64 syscall;
	__asm__ volatile("mv %0, a0":"=r"(syscall));
	switch (syscall) {
		case 0x01:
			__mem_alloc();
			break;
		case 0x02:
			__mem_free();
			break;
	}
}

void __push_back(struct __list* list, void* elem) {
	struct __node* newNode = __MA_allocate(sizeof(struct __node));
	newNode->d = elem;
	newNode->next = NULL;
	if(!list->tail) {
		list->tail = newNode;
		list->head = newNode;
	} else {
		list->tail->next = newNode;
		list->tail = newNode;
	}
}

void* __pop_front(struct __list* list) {
	if(!list) return NULL;
	struct __node* front = list->head;
	if(!front) return NULL;
	if(list->head == list->tail) list->tail=NULL;
	list->head = list->head->next;
	void* data = front->d;
	__MA_free(front);
	return data;
}