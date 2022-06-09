#ifndef OS1_PROJEKAT_SYSCALL_CPP_H
#define OS1_PROJEKAT_SYSCALL_CPP_H

#include "syscall_c.h"

void* operator new(size_t);

void operator delete(void*);

class Thread {
public:
	Thread(void (* body)(void*), void* arg);

	virtual ~Thread();

	int start();

	static void dispatch();

	static int sleep(time_t);

protected:
	Thread();

	virtual void run() {}

	friend void threadWrapper(void*);

private:
	thread_t myHandle;
	void* argument = nullptr;
	sem_t startThread;
	void (* body)(void*) = nullptr;
};

class Semaphore {
public:
	Semaphore(unsigned init = 1);

	virtual ~Semaphore();

	int wait();

	int signal();

private:
	sem_t myHandle;
};

class PeriodicThread : public Thread {
protected:
	PeriodicThread(time_t period);

	virtual void periodicActivation() {}
};

class Console {
public:
	static char getc();

	static void putc(char);
};

#endif //OS1_PROJEKAT_SYSCALL_CPP_H
