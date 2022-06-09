#include "../h/syscall_cpp.h"

void* operator new(size_t size) {
	return mem_alloc(size);
}

void operator delete(void* ptr) {
	mem_free(ptr);
}

Thread::~Thread() { }

void Thread::dispatch() {
	thread_dispatch();
}

int Thread::sleep(time_t t) {
	return time_sleep(t);
}

Semaphore::Semaphore(unsigned int init) {
	sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
	sem_close(myHandle);
}

int Semaphore::wait() {
	return sem_wait(myHandle);
}

int Semaphore::signal() {
	return sem_signal(myHandle);
}
