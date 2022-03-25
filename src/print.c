#include "../h/print.h"

void printstr(const char* str){
	char c;
	uint32 i;
	for(i = 0, c = str[i] ; c != '\0' ; i++) __putc(c);
}

void printunum(uint64 num){
	
}
