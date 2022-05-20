#ifndef _USRLIB_H_
#define _USRLIB_H_

#include "../lib/hw.h"

#define NULL 0
#define CRUMBS 4

uint64 __align(uint64 what, uint64 to);

void __init_system();

#endif