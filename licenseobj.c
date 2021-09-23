//License object
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "licenseobj.h"



extern int *nlicenses;


int getlicense(void){
	//Blocks until a license is available
	if(nlicenses>0){
		nlicenses--;
	}
	else{
		wait(nlicenses);
		return 0;
	}
	return 0;
}

int returnlicense(void){
	//Increments the number of available licenses
	nlicenses++;
	return 0;
}

int initlicense(int number){
	//Performs any needed initialization of the license object
	*nlicenses = number;
	return 0;
}

int addtolicenses(int n){
	//Adds n licenses to the number available
	nlicenses+=n;
	return 0;
}

int removelicenses(int n){
	//Decrements the number of licenses by n
	nlicenses-=n;
	return 0;
}

int logmsg(const char * msg){
	//Write the specified message to the log file.  There is only one log file.
	//This functino will treat the log file as a critical resource.  Append the message and close the file.
	return 0;
}
