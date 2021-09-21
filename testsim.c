/*File: testsim.c
 * Author: Patrick Carra
 * Class: CS-4760
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int sleep_time = atoi(argv[1]);
    int repeat_factor = atoi(argv[2]);
    if (argc <=2){
	    fprintf(stderr, "Usage: %s sleep_time repeat_factor\n", argv[1]);
	    exit(1);
    }
    while(repeat_factor-- > 0){
	    sleep(sleep_time);
    }
    printf("Process ID = %d \n", getpid());
    return sleep_time;
}
