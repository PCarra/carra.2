/* File: runsim.c
 * Author: Patrick Carra
 * Class: CS-4760
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_CANON 20

void docommand(char *cline);

void print_usage(char *argv[]){
	fprintf(stderr, "Usage: %s [number of processes]\n", argv[0]);
}

void docommand(char *cline){
	//fork a child (a grandchild of the original), Grandchild calls makeargv on cline and calls execvp on the resulting array
	pid_t grchildpid = 0;
	//child forks grandchild to execute command
	//runsim checks to see if any of the children have finished waitpid with WNOHANG option) and when that happens it returnlicense
	grchildpid=fork();
	system(cline);
	//wait for this child and then return license to the license object
}

int main (int argc, char *argv[]) {
	pid_t childpid = 0;
	pid_t wpid = 0;
	int pr_limit;//specifies the maximum number of children allowed to execute at a time (int value passed in on cmd line)
	int status, child;

	//error checking for number argument doesn't work 
	if (argc != 2){  //check for valid number of command-line arguments 
		print_usage(argv);
 		return 1;
 	}
 
	pr_limit = atoi(argv[1]);
	if(pr_limit>0){
		int pr_count=0;//number of children
		char buffer[MAX_CANON];
		while(fgets(buffer, MAX_CANON, stdin)!=NULL){
			//request a license from the license object
			//fork a child that calls docommand
			if((childpid=fork())<=0){
				pr_count++;
				return 0;
			}
			//waits for a child process to finish if the limit is reached
			//if(pr_count == pr_limit){
			if((wpid=waitpid(-1,NULL, WNOHANG)!=0)){
				wait(NULL);
				pr_count--;
			}
			//pass the input string from stdin to docommand which will execl the command (child)
			char progstr[20];
			strcpy(progstr, "./");
			strcat(progstr, buffer);
			docommand(progstr);
		}
		//After EOF on stdin, wait for all the remaining children to finish and then exit
		/*
		while((wpid=wait(&status))>0){
			printf("Child Terminated.\n");
		}
		*/
		while(1){
			child = wait(NULL);

			if ((child == -1) && (errno != EINTR)){
			break;
			}
		}
	}
	return 0;
}
