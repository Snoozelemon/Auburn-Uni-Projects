#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int wordCount(const char word[]){
	
	static int count = 0;
	const char *p = word;
	int inword = 0;

	do switch(*p){
		case '\0':
		case ' ':case '\t':case '\n':case '\r':
		if(inword){ inword = 0;count++;}
		break;
		default: inword = 1;
	}while(*p++);

	return count;
}
