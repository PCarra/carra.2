/*File: testsim.c
 * Author: Patrick Carra
 * Class: CS-4760
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
	time_t current_time;
	struct tm * time_info;
	char timeString[9];

	int sleep_time = atoi(argv[1]);
    	int repeat_factor = atoi(argv[2]);
    	if (argc <=2){
	  	fprintf(stderr, "Usage: %s sleep_time repeat_factor\n", argv[1]);
		    exit(1);
    	}
    	for(int i=1; i<repeat_factor; i++){
		time(&current_time);
		time_info = localtime(&current_time);
		strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
		//sleep(sleep_time);
    	    	//Ouptut to logfile in format:Time PID Iteration# of NumberOfIterations
		//printf("testsim %d %d\n", sleep_time, repeat_factor);
    	    	printf("%s %d %d\n", timeString, getpid(), i);
    	}
    	return sleep_time;
}
