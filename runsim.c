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
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <signal.h>
#include "config.h"
#include <time.h>

#define MAX_CANON 20
#define IPC_RESULT_ERROR (-1)
#define BUF_SIZE 1024
int *nlicenses;
time_t startTime;
time_t endTime;

void docommand(char *cline);

void print_usage(char *argv[]){
	fprintf(stderr, "Usage: %s [number of processes]\n", argv[0]);
}

void docommand(char *cline){
	char delim[] = " ";
	char *binaryPath = strtok(cline, delim);
	char *arg1 = strtok(NULL, delim);
	char *arg2 = strtok(NULL, delim);
	execl(binaryPath, binaryPath, arg1, arg2, NULL);

 
	//fork a child (a grandchild of the original), Grandchild calls makeargv on cline and calls execvp on the resulting array
	//pid_t grchildpid;
	//grchildpid=fork();
	//runsim checks to see if any of the children have finished waitpid with WNOHANG option) and when that happens it returnlicense
	//system(cline);
	//wait for this child and then return license to the license object
}

int destroy_mem_block(){
	if(nlicenses==IPC_RESULT_ERROR){
		return NULL;
	}
	return (shmctl(*nlicenses, IPC_RMID, NULL) != IPC_RESULT_ERROR);
}

static void catch_signal(int sig){
	printf("Caught signal %d exiting....\n", sig);
	destroy_mem_block();
	exit(1);
}

int main (int argc, char *argv[]) {
	time(&startTime);
	pid_t childpid = 0;
	pid_t wpid = 0;
	int pr_limit, pr_count;//specifies the maximum number of children allowed to execute at a time (int value passed in on cmd line)
	int shmid, child;
	key_t key;

	//error checking for number argument doesn't work 
	if (argc != 2){  //check for valid number of command-line arguments 
		print_usage(argv);
 		return 1;
 	}
	//create a key
	key = (key_t)1221;
	if (key==IPC_RESULT_ERROR){
		shmid= IPC_RESULT_ERROR;
	}
	//get an id to the shared segment
	else{
	        shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT);
	}
	//allocate a size of int to the pointer nlicenses and attach the shared segment to the pointer
	nlicenses = malloc(sizeof(int));
	nlicenses = shmat(shmid, NULL, 0);
	if (nlicenses == (void *) -1){
		perror("Shared memory attach");
		return 1;
	}
	//populate shared memory with command line argument for the number of available licenses
	*nlicenses = atoi(argv[1]);
	signal(SIGINT, catch_signal);
	//fork a child
	pr_count=0;
	char buffer[MAX_CANON];
	while(fgets(buffer, MAX_CANON, stdin)!=NULL){
		if(difftime(time(&endTime), startTime) >= MAX_SECONDS){
			printf("Exceeded time limit");
			raise(SIGINT);
		}
		//request a license from the license object
                //waits for a child process to finish if the limit is reached
                if(pr_count==NUM_PROCESSES){
			printf("Waiting on max processes\n");
                	wait(NULL);
                        pr_count--;
                }
                //fork a child that calls docommand
		child = fork();
                if(child<0){
                	perror("Forking failed\n");
                        exit(1);
                }
                pr_count++;
                if(childpid==0){
                	//pass the input string from stdin to docommand which will execl the command (child)
			printf("Child %d calling testsim\n", child);
                        char progstr[20];
                        strcpy(progstr, "./");
                        strcat(progstr, buffer);
                        docommand(progstr);
                }
                if((wpid=waitpid(-1,NULL, WNOHANG)!=0)){
                	pr_count--;
                }
        }
	/*
	if(shmdt(nlicenses) == -1){
		perror("shmdt");
		return 1;
	}
	*/
	destroy_mem_block();
	return 0;
}
