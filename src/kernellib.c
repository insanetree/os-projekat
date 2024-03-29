#include "../h/kernellib.h"
#include "../h/MemoryAllocator.h"
#include "../h/syscall_handlers.h"
#include "../h/scheduler.h"
#include "../h/tcb.h"
#include "../h/semaphore.h"
#include "../h/sleeper.h"
#include "../lib/console.h"
#include "../h/slab.h"
#include "../h/cache.h"
#include "../h/buddy.h"

kmem_cache_t* tcb_cache;
const char* tcb_cache_name = "TCB Cache";
kmem_cache_t* stack_cache;
const char* stack_cache_name = "Stack Cache";
kmem_cache_t* semaphore_cache;
const char* semaphore_cache_name = "Semaphore Cache";
kmem_cache_t* buffer_cache[13]={NULL};
const char* buffer_cache_name[13] = {"size5", "size6", "size7", "size8",
				     "size9", "size10", "size11", "size12",
				     "size13", "size14", "size15", "size16",
				     "size17"};

void __handle_syscall();
void __interrupt();
void __interrupt_handler();

uint64 __align(uint64 what, uint64 to) {
	return what % to ? what + (to - what % to) : what;
}

inline void __init_system() {
	//memory initialization
	__MA_memory_init();
	uint64 buddySpace = 1<<24;
	kmem_init(__MA_allocate(buddySpace), buddySpace/BLOCK_SIZE);
	tcb_cache = kmem_cache_create(tcb_cache_name, sizeof(struct __tcb), NULL, NULL);
	stack_cache = kmem_cache_create(stack_cache_name, DEFAULT_STACK_SIZE, NULL, NULL);
	semaphore_cache = kmem_cache_create(semaphore_cache_name, sizeof(struct __semaphore), NULL, NULL);
	for(int i = 0 ; i < 13 ; i++) {
		buffer_cache[i] = kmem_cache_create(buffer_cache_name[i], 1<<(i+5), NULL, NULL);
		buffer_cache[i]->slotsInSlab=4;
	}
	__init_scheduler();

	//__init_console();

	//set interrupt_handler address in stvec
	__asm__ volatile("csrw stvec, %0 ": : "r" (&__interrupt));

	//uint64 ueie = 0x322;

	//__asm__ volatile("csrw sie, %0"::"r"(ueie));

	//enable interrupt ALWAYS AT THE END
	__asm__ volatile("csrs sstatus, 0x02");
}
void shrink_caches();
void __interrupt_handler() {
	uint64 a0, a1, a2, a3, sstatus;
	__asm__ volatile("mv %0, a0":"=r"(a0));
	__asm__ volatile("mv %0, a1":"=r"(a1));
	__asm__ volatile("mv %0, a2":"=r"(a2));
	__asm__ volatile("mv %0, a3":"=r"(a3));
	if(running->body == NULL)
		__clear_exit_stack();

	__asm__ volatile("mv a0, %0"::"r"(a0));
	__asm__ volatile("mv a1, %0"::"r"(a1));
	__asm__ volatile("mv a2, %0"::"r"(a2));
	__asm__ volatile("mv a3, %0"::"r"(a3));

	uint64 scause;
	uint64 sepc;
	__asm__ volatile("csrr %0, sepc":"=r"(sepc));
	__asm__ volatile("csrr %0, scause":"=r"(scause));
	__asm__ volatile("csrr %0, sstatus":"=r"(sstatus));
	switch(scause) {
		case 0x8000000000000001UL:
			__sleep_pop();
			running->time++;
			if(running->time % DEFAULT_TIME_SLICE == 0) {
				__thread_dispatch();
			}
			break;
		case 0x8000000000000009UL:
			console_handler();
			break;
		case 8:
		case 9:
			//ecall
			sepc+=4;
			__handle_syscall();
			__asm__ volatile("sd a0, 80(fp)");
			break;
	}
	__asm__ volatile("csrci sip, 0x02");
	__asm__ volatile("csrw sepc, %0"::"r"(sepc));
	__asm__ volatile("csrw sstatus, %0"::"r"(sstatus));
	shrink_caches();
}

void __handle_syscall() {
	uint64 syscall;
	__asm__ volatile("mv %0, a0":"=r"(syscall));
	switch (syscall) {
		case 0x01:
			__mem_alloc_handler();
			break;
		case 0x02:
			__mem_free_handler();
			break;
		case 0x11:
			__thread_create_handler();
			break;
		case 0x12:
			__thread_exit_handler();
			break;
		case 0x13:
			__thread_dispatch();
			break;
		case 0x21:
			__sem_open_handler();
			break;
		case 0x22:
			__sem_close_handler();
			break;
		case 0x23:
			__sem_wait_handler();
			break;
		case 0x24:
			__sem_signal_handler();
			break;
		case 0x31:
			__time_sleep_handler();
			break;

	}
}

int __push_back(struct __list* list, void* elem) {
	struct __node* newNode = __MA_allocate(sizeof(struct __node));

	if(newNode == NULL) {
		return -1;
	}

	newNode->d = elem;
	newNode->next = NULL;
	if(!list->tail) {
		list->tail = newNode;
		list->head = newNode;
	} else {
		list->tail->next = newNode;
		list->tail = newNode;
	}

	return 0;
}

void* __pop_front(struct __list* list) {
	if(!list) return NULL;
	struct __node* front = list->head;
	if(!front) return NULL;
	list->head = list->head->next;
	if(list->head == NULL) list->tail = NULL;
	void* data = front->d;
	__MA_free(front);
	return data;
}