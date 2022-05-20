#include "../h/kernellib.h"
#include "../h/MemoryAllocator.h"

uint64 __allign(uint64 a, uint64 b) {
	return a%b?a + (b - a%b) : a;
}

void __init_system() {
	__MA_memory_init();
}