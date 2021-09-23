//License object
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


struct License {
        int nlicenses;
        char logfile[];
};


int getlicense(void);
int returnlicense(void);
int initlicense(int number);
int addtolicenses(int n);
int removelicenses(int n);
int logmsg(const char * msg);
