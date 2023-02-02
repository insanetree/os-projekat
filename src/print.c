#include "../h/print.h"

void printstr(const char* str){
	char c;
	uint32 i;
	i = 0;
	c = str[0];
	while(c != '\0'){
		putc(c);
		i++;
		c = str[i];
	}
}

void printunum(uint64 num){
	static const int decplc = 20;
	int i;
	char str[decplc];
	char* p = str;
	str[decplc-1] = '\0';
	for(i = decplc-2 ; i >= 0 ; i--)
	{
		str[i] = '0' + num % 10;
		num /= 10;
	}
	while(*p == '0'&&p!=&str[decplc-2])p++;
	printstr(p);
}
