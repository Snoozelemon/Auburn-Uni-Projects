#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>


char *file_uploader(const char *fileName){

	FILE *fptr;
	static char text[1000];
	fptr = fopen(fileName, "r");  /*reading the content from "fileName"*/
	
	if(fptr == NULL){
		return 0;	/*error checking if file didnt open*/
	}
	
	fgets(text,1000,fptr);	/*Read the file"*/
	return text;
	fclose(fptr);	
}
