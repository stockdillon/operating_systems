
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
	//char current_char;
	
	char option_error[] = "Invalid option, valid options are -t, -n, -h \n";
	
	char no_file[] = "Invalid file name... \n";
	
	
	
	for(int i=1; i< argc; i++){
		
		memset(buf, 0, sizeof(buf));		
		
		int fd = open(argv[i], O_RDONLY);
		
		temp = *(argv+i);
		
		if(fd == (-1)){ // if file can not be opened
			if(temp[0]!= '-'){ // if the argument is not an option...
				write(2, no_file, strlen(no_file)); // display error message in stderr
				continue;
			}
		}
		
		if(temp[0] == '-'){ // if the argument is an option
			//temp = *(argv+i);        //uncomment
			if(temp != "-t" && temp != "-n" && temp != "-h"){ // if option is invalid...
				write(2, option_error, strlen(option_error)); 
				continue;
			}
			if(temp == "-t"){
				tset = true;
				continue;
			}
			
			if(temp == "-n"){
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
			
		
		//int line_end = 0;
		
		/*
		int size = read(fd, buf, 512);
		while(size > 0){
			write(1, buf, size);
		}
		*/
		
		
		
		
		
		unsigned size = read(fd, buf, 512);
		bool end = false;
		
		if(nset){
				
			int count =1;
			char line[5];
			while(!end){
				if(size < 512){
					end = true;
				}
				
				int prev =0;
				
				for(unsigned j=0; j< size; j++){
					if(buf[j] == '\n'){
						write(1, "\n", strlen("\n"));
						sprintf(line, "%.4d " , count);
						write(1, line, 5);
						write(1, buf+prev, (j-prev));
						//write(1, "\n", strlen("\n"));
						prev = j;
						count++;
					}
				}
				
				//write(1, buf, 512);
				size = read(fd, buf, 512);
				
			}
			
		}
		
		
		if(!nset){
				
			//int count =0;
			//char line[5];
			
			while(!end){
				if(size < 512){
					end = true;
				}
	
				
				write(1, buf, 512);
				size = read(fd, buf, 512);
			}
				
			
		}
		
		
		
		
		
		
		
		
		
		
		
		/*
		while(read(fd, buf, 512) > 0){
			
			for(unsigned j=0;j<strlen(buf); j++){
				current_char = buf[j];
				if(current_char == '\n'){
					write(1, "LINE: ", strlen("LINE: "));
					write(1, buf, j);
				}
			}
			
			//write(1, buf, strlen(buf));
		}
		*/
		
		
		/*
		while(read(fd, buf, 512) > 0){
			for(int j=0; j<strlen(
			write(1, buf, strlen(buf));
		}
		*/
		
		

		

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

