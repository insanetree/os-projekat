#include "../h/usrlib.h"

uint64 __align(uint64 a, uint64 b) {
	return a + (b - a%b);
}