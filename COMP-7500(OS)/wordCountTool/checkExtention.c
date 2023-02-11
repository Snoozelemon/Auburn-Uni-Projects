#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int check_ext(const char *filename){
    const char arr[] = "file.txt";
    const char *dot1 = strrchr(arr, '.');
	const char *dot2 = strrchr(filename, '.');
    if(strcmp(dot1,dot2)){
        printf("Please check the file extension!\nInput file just be '.txt' file\n");
        return 0;
    }else{
        return 1;
    }
}
