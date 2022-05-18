#include "../h/usrlib.h"

uint64 __allign(uint64 a, uint64 b) {
	return a%b?a + (b - a%b) : a;
}