
/*
Dillon Stock stockdil@basset
2017-01-29
untitled.cpp

Your Comments
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <string>

//#include <boolalpha>
using std::cout; using std::endl; using std::cin;
using namespace std;

int main(int argc, char *argv[]){
	
	char help_menu[] = "Enter a series of filenames with contents you would like to have displayed. \n"
	"Use the option '-t' to display the title of each file along with the contents of the file. \n"
	"Use the option '-n' to display line numbers along with the contents of the file. \n"
	"Use the option '-h' to display a help menu. \n";
		
	bool tset = false;
	bool nset = false;
	string temp;
	
	
	char buf[512];
	char current_char;
	
	
	
	for(int i=1; i< argc; i++){				
		
		int fd = open(argv[i], O_RDONLY);
		
		temp = *(argv+i);
		
		
		if(fd == (-1)){
			//temp = *(argv+i);        //uncomment
			if(temp[0] == '-' && temp != "-t" && temp != "-n" && temp != "-h"){
				cout << "Invalid command, valid commands are -t, -n, -h '" << temp << "'" << endl;
				continue;
			}
			if(tset || temp == "-t"){
				tset = true;
				continue;
			}
			
			if(nset || temp == "-n"){
				nset = true;
				continue;
			}
			
			if(temp == "-h"){
				// writing help message...
				write(2, "\n", strlen("\n"));
				write(2, help_menu, strlen(help_menu));
				write(2, "\n", strlen("\n"));
				continue;
			}
		}
		

////////////////////////////////////////////////////////
		// writing file header...
		
		//tset = true;
		
		if(tset){

			char * temp2 = new char [temp.length()+1];
			std::strcpy(temp2, temp.c_str());
			
			char * colons = new char [temp.length()+1];
			std::strcpy(colons, std::string(strlen(*(argv+i)), ':').c_str());		
			
			write(1, colons, strlen(temp2));
			write(1, "\n", strlen("\n"));
			write(1, temp2, strlen(temp2));
			write(1, "\n", strlen("\n"));
			write(1, colons, strlen(temp2));
			write(1, "\n\n", strlen("\n\n"));
		}
////////////////////////////////////////////////////////
			
		
		int line_end = 0;
		
		while(read(fd, buf, 512) > 0){
			write(1, buf, strlen(buf));
		}
		close(fd);
		

/*		
//////////////////////////////////////////////////////////
		// writing help message...
		write(2, "\n", strlen("\n"));
		write(2, help_menu, strlen(help_menu));
		write(2, "\n", strlen("\n"));
//////////////////////////////////////////////////////////
*/

		
	}
	
	return 0;
}

