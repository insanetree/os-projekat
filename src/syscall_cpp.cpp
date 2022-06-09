#include "../h/syscall_cpp.hpp"

void* operator new(size_t size) {
	return mem_alloc(size);
}

void operator delete(void* ptr) {
	mem_free(ptr);
}

void threadWrapper(void* arg) {
	Thread* thread = (Thread*)arg;
	sem_wait(thread->startThread);
	if(thread->body == nullptr)
		thread->run();
	else
		thread->body(thread->argument);
}

Thread::Thread(void (* body)(void*), void* arg) : argument(arg), body(body) {
	sem_open(&startThread, 0);
	thread_create(&myHandle, threadWrapper, (void*)this);
}

Thread::Thread() {
	sem_open(&startThread, 0);
	thread_create(&myHandle, threadWrapper, (void*)this);
}

int Thread::start() {
	return sem_signal(startThread);
}

Thread::~Thread() {
	sem_close(startThread);
}

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

char Console::getc() {
	return ::getc();
}

void Console::putc(char c) {
	::putc(c);
}