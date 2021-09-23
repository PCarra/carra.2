/* File: runsim.c
 * Author: Patrick Carra
 * Class: CS-4760
 */

#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <errno.h>
#include "config.h"
#include "licenseobj.h"

#define MAX_CANON 20
#define IPC_RESULT_ERROR (-1)
#define BUF_SIZE 1024
#define IP_RESULT_ERROR 1
int *nlicenses;
int PID_child[100];
int free_pos=0;

int detachandremove(int shmid, void *shmaddr){
	int error=0;
	if (shmdt(shmaddr)==-1)
		error = errno;
	if ((shmctl(shmid, IPC_RMID, NULL)==-1) && !error)
		error = errno;
	if (!error)
		return 0;
	errno = error;
	return -1;
}

void print_usage(char *argv[]){
	fprintf(stderr, "Usage: %s [number of processes]\n", argv[0]);
}

void docommand(char *cline){
	//fork a child (a grandchild of the original), Grandchild calls makeargv on cline and calls execvp on the resulting array
	pid_t childpid = 0;
	pid_t wpid = 0;
 	int grchild_count = 0;
	char errstr[255];
	if((childpid=fork())==0){
		char delim[] = " ";
		char *binaryPath = strtok(cline, delim);
		char *arg1 = strtok(NULL, delim);
		char *arg2 = strtok(NULL, delim);
		char *arg3 = strtok(NULL, delim);
		execl(binaryPath, binaryPath, arg1, arg2, arg3, NULL);
	}
	//runsim checks to see if any of the children have finished waitpid with WNOHANG option) and when that happens it returnlicense
	else{
		while((wpid=waitpid(-1,NULL, WNOHANG))){
			if((childpid==-1) && (errno != EINTR)){
				printf("Waiting on grand child to exit...\n");
				//waits on children
               			grchild_count--;
				break;
			}
        	}		
		//wait for this child and then return license to the license object
	}
        if(childpid<0){
		printf(errstr, "Error in creating grandchild process");
               	perror(errstr);
        }
        
}

void INThandler(int sig){
	printf( "Caught signal %d exiting....\n", sig);
	signal(SIGQUIT, SIG_IGN);
	kill(0, SIGTERM);
	exit(0);
}

static void myhandler(int s){
	char aster = '*';
	int errsave;
	errsave = errno;
	write(STDERR_FILENO, "Exceeded time limit....", 1);
	raise(SIGINT);
	errno = errsave;
}

static int setupinterrupt(void){
	struct sigaction act;
	act.sa_handler = myhandler;
	act.sa_flags=0;
	return (sigemptyset(&act.sa_mask) || sigaction(SIGPROF, &act, NULL));
}

static int setupitimer(void){
	struct itimerval value;
	value.it_interval.tv_sec = MAX_SECONDS;
	value.it_interval.tv_usec = 0;
	value.it_value = value.it_interval;
	return (setitimer(ITIMER_PROF, &value, NULL));
}


int main (int argc, char *argv[]) {
	signal(SIGINT, INThandler);
	if (setupinterrupt() == -1){
		perror("Failed to set up handler for SIGPROF");
		return 1;
	}
	if (setupitimer() == -1){
		perror("Failed to set up the ITIMER_PROF interval timer");
		return 1;
	}

	pid_t childpid = 0;
	pid_t wpid = 0;
	int shmid, pr_count;//specifies the maximum number of children allowed to execute at a time (int value passed in on cmd line)
	char errstr[255];

	//error checking for number argument doesn't work 
	if (argc != 2){  //check for valid number of command-line arguments 
		print_usage(argv);
 		return 1;
 	}

	//get an id to the shared segment
	if ((shmid=shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT))==-1){
		perror("Failed to create shared memory segment");
		return 1;
	}
	//allocate a size of int to the pointer nlicenses and attach the shared segment to the pointer
	if ((nlicenses = (int *)shmat(shmid, NULL, 0)) == (void *)-1){
		perror("Failed to attach shared memory segment");
		if(shmctl(shmid, IPC_RMID, NULL)==-1)
			perror("Failed to remove memory segment");
		return 1;
	}
	struct License *license;
	//populate shared memory with command line argument for the number of available licenses
	*nlicenses = atoi(argv[1]);

	//printf("Number of Licenses Available: %d\n", *nlicenses); 
	//fork a child
	pr_count=0;
	char buffer[MAX_CANON];
	while(fgets(buffer, sizeof buffer, stdin)!=NULL){
		//request a license from the license object
		//getlicense();
		//printf("Number of Licenses Available: %d\n", *nlicenses); 
		//licenseobj.getlicense()
                //waits for a child process to finish if the limit is reached
                if(pr_count==NUM_PROCESSES){
                	childpid = wait(NULL);
			if(childpid!=-1)
				printf("%ld Waited for child with pid %d because max processes reached\n", (long)getpid(), childpid);
                        pr_count--;
                }
                //fork a child that calls docommand
		//printf("%ld Forking main child\n", (long)getpid());
                if((childpid=fork())==0){
			pr_count++;
			//printf("I am a child %ld\n", (long)getpid());
                	//pass the input string from stdin to docommand which will execl the command (child)
                        char progstr[20];
                        strcpy(progstr, "./");
                        strcat(progstr, buffer);
                        docommand(progstr);
			//printf("Licenses available: %d", *nlicenses);
			break;
		}
		else{
			PID_child[free_pos]=(long)childpid;
			free_pos++;
			//printf("I am a parent %ld\n", (long)getpid());
			while((wpid=waitpid(-1,NULL, WNOHANG))){
				//printf("%ld Waiting on main child to exit...\n", (long)getpid());
				//waits on children
                		pr_count--;
				//break;
                	}
		}
		if(childpid<0){
			perror("Failed to create child proces");
			if(detachandremove(shmid, nlicenses)==-1)
				perror("Failed to destroy shared memory segment");
			return -1;
                }
	}
	if(detachandremove(shmid, nlicenses) == -1) {
		perror("Failed to destroy shared memory segment");
		return 1;
	}
	return 0;
}
/*
                }
		//returnlicense();
		//printf("Number of Licenses Available: %d\n", *nlicenses);
        }
	if(childpid!=0)
	        printf("%ld Process count: %d\n", (long)getpid(), pr_count);
	if(shmdt(nlicenses) == -1){
		perror("shmdt");
		return 1;
	}
	destroy_mem_block();
	return 0;
}

*/
