#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<unistd.h>

#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>

#include "file_uploader.c"
#include "wordCount.c"
#include "animation.c"
#include "checkExtention.c"

#define BUFFER_SIZE 1000


int read_end = 0;
int write_end = 1;


int main(int argc, char *argv[]){
	/*if user doesnt enter file name*/

	if(argc == 0){
		printf("Please enter a file name.\nUsage: ./pwortcount <file_name>");
		return 0;
	}
	
	/*if user enters no input*/
	if(argc!=2){
		if(argc!=0){
			printf("Please enter a file name.\nUsage: ./pwordcount <file_name>\n");
		return 1;
		}
	}
	

	if(argc == 2){
		/*Checking user errors*/

		if(check_ext(argv[1]) == 0){
			return 2;
		}
		else if(!file_uploader(argv[1])){
			printf("Unable to  upload the file\nPlease check file name\n");
			return 2;
		}
		else{
			printf("Opening the input file....\n");
			print_progress(5);	/*Loading animation*/		
		}

		/*Loading the file into buffer*/
		
		char w_msg[BUFFER_SIZE];
		char r_msg[BUFFER_SIZE];

		strcpy(w_msg,file_uploader(argv[1]));
	
		int sizeOfMsg = strlen(w_msg)+1;
		int fd1[2], fd2[2];

		/*Unable to establish Pipe conncetion then*/
		if(pipe(fd1) == -1){ 
			return 3;
		}
		if(pipe(fd2) == -1){
			return 4;
		}

		/*forking*/

		int id = fork();
		if(id <0){
			printf("Forking failed");
			return 5;
		}

		
		if(id !=0){
						/*Parent process*/
			
			close(fd1[read_end]); /*closing the read end of the pipe*/
			printf("Process 1: Sending the input file....\n");

			/*Writing the contents*/
			write(fd1[write_end],w_msg,sizeOfMsg);

			close(fd1[write_end]);

			wait(0); /*waiting for child process to return result*/
			
			int result[5];
			/*receiving the data from child process*/
			close(fd2[write_end]);
			read(fd2[read_end], result, sizeof(result));
			print_progress(3);
			if(result == 0){
				printf("0 words, The file is empty.\n");
				}else if(result[0] == 1){
					printf("There is only one word in the file\n");
				}else{
					printf("Process 1: There are %d words in the file\n", result[0]);
				}
				
		}else if(id == 0){

							/*Child process*/
			print_progress(3);
			printf("Process 2: Receiving the input file....\n");
			close(fd1[write_end]);
			read(fd1[read_end], r_msg, BUFFER_SIZE);
			close(fd1[read_end]);

			/*counting the words*/
			print_progress(4);
			printf("Process 2: Counting words now....\n");
			int count;
			count = wordCount(r_msg);
			int number[4];
			number[0] = count;
			close(fd2[read_end]);

			/*sending results*/
			print_progress(3);
			printf("Process 2: Sending back the data to process 1 now....\n");
			write(fd2[write_end], number, sizeof(int));
			close(fd2[write_end]);
			
		}

	}
	return 0;
}
