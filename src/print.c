#include "../h/print.h"

void printstr(const char* str){
	char c;
	uint32 i;
	i = 0;
	c = str[0];
	while(c != '\0'){
		__putc(c);
		i++;
		c = str[i];
	}
}

void printunum(uint64 num){
	
}
