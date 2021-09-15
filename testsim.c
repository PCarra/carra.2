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
    int i = 0;
    //pid_t mypid = getpid();
    
    for (i=1; i<=repeat_factor; i++) {
	printf("Call license object, repeating %d, sleeping %d\n", repeat_factor, sleep_time);
        //sleep(sleep_time);
    }
    
    //printf("TESTSIM: pid %ld terminating.\n", mypid);
    printf("Testsim: terminating pid\n");
    return 0;
}
