/* File: runsim.c
 * Author: Patrick Carra
 * Class: CS-4760
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>

void print_usage(char *argv[]){
	fprintf(stderr, "Usage: %s [number of processes]\n", argv[0]);
}

bool isNumber(char number[]){
	int i = 0;
	if(number[0]=='-')
		i=1;
	for(; number[i]!=0; i++){
		if(!isdigit(number[i]))
			return false;
	}
	return true;
}

int main (int argc, char *argv[]) {
	pid_t childpid = 0;
	pid_t wpid = 0;
 	int i, n, option, pr_limit, status;
	int hflag = 0;
	int nflag = 0;
	int pr_count=0;

	while ((option = getopt(argc, argv, "hn"))!=-1){
		switch(option){
			case 'h':
				hflag = 1;
				break;
			case 'n':
				nflag = 1;
				break;
			default:
				hflag = 1;
				break;
		}
	}

	if((hflag==1) && (nflag==0)){
		print_usage(argv);
		return 0;
	}
	else if((nflag==1) && (hflag==0)){
		if(argv[2]!=NULL && ((pr_limit=atoi(argv[2]))>0)){
			char program[20];
			char params[20];
			FILE *fp = fopen("testing.data", "r");
			if(fp==NULL){
				perror("Error opening file: ");
				return 0;
			}
			while(fgets(params, 20, fp)!=NULL){
				if(pr_count == pr_limit){
					wait(NULL);
					pr_count--;
				}
				strcpy(program, "./");
				strcat(program, params);
				if((childpid = fork()) <=0){
					//printf("forking process....");
					printf("%s, %s", program, params);
					system(program);
					return 0;
				}
				pr_count++;
			}
			fclose(fp);
			while(wpid=wait(&status)>0){
				printf("%s: All children terminated.\n", argv[0]);
				return 0;
			}
		}
		else {
			printf("%s: ERROR: invalid option selection\n", argv[0]);
			print_usage(argv);
			return 0;
		}
		return 0;
	}
 	/*
	//error checking for number argument doesn't work 
	if (argc != 2){  check for valid number of command-line arguments 
		if (!isNumber(argv[1])) {
			print_usage(argv);
 			return 1;
		}
 	}
 
	n = atoi(argv[1]);
	for (i = 1; i < n; i++)
		if ((childpid = fork()) <= 0)
			break;
	fprintf(stderr, "i:%d process ID:%ld parent ID:%ld child ID:%ld\n",
		i, (long)getpid(), (long)getppid(), (long)childpid);
	return 0;
	*/
}
