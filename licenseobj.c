//License object

class License {
	public:
		int nlicenses; 
		int getlicense(void);
		int returnlicense(void);
		int initlicense(void);
		addtolicenses(int n);
		removelicenses(int n);
		logmsg(const char * msg);
	private:
}


class License : Public License {
	public:
		int getlicense(void){
			//Blocks until a license is available
			return 0;
		}
		int returnlicense(void){
			//Increments the number of available licenses
			return 0;
		}
		int initlicense(void){
			//Performs any needed initialization of the license object
			return 0;
		}
		addtolicenses(int n){
			//Adds n licenses to the number available
			return 0;
		}
		removelicenses(int n){
			//Decrements the number of licenses by n
			return 0;
		}
		logmsg(const char * msg){
			//Write the specified message to the log file.  There is only one log file.
			//This functino will treat the log file as a critical resource.  Append the message and close the file.
			return 0;
		}
	private:
};
