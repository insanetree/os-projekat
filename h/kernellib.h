#ifndef _USRLIB_H_
#define _USRLIB_H_

#include "../lib/hw.h"
#include "../lib/console.h"

#define NULL 0
#define CRUMBS 4

#ifdef __cplusplus
extern "C" {
#endif

uint64 __align(uint64 what, uint64 to);

void __init_system();

struct __node {
	void* d;
	struct __node* next;
};

struct __list {
	struct __node* head;
	struct __node* tail;
};

void __push_back(struct __list* list, void* elem);

void* __pop_front(struct __list* list);

#ifdef __cplusplus
}
#endif


#endif