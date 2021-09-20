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
    int i = 0;
    for (i=1; i<=repeat_factor; i++) {
        //sleep(sleep_time);
	fprintf(stderr, "Process ID: %d is sleeping \n", getpid());
    }
    return 0;
}
