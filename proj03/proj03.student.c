#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>





using std::cout; using std::endl; using std::cin;
using namespace std;

void display_prompt(int commandNum, string userid){
	cout << "<" << commandNum << " " << userid << ">";
}

int main(int argc, char *argv[], char** variables){
	//char id[30];
	char id[256];
	cuserid(id);
	string id_string(id);
	
	int count = 1;
	string command  = "none";
	string tokens;
	string token;
	
	while(true){
		display_prompt( count, id_string);
		count++;
		getline(cin, tokens);
		istringstream tokenstream(tokens);
		
				
		// if 'tokenstream' is empty, there is no command; continue prompting user for command
		// the FIRST TOKEN in the line (tokenstream) is the COMMAND
		if(!(tokenstream >> command)){
			// if the user enters an empty line, we do NOT increment the command count
			// since the count increments by default, we must decrement it
			count--;
			continue;
		}
		
		
		//////////////// displaying all tokens following command ////////////
		/*
		cout << "----------tokens ---------- : " << endl;
		while(tokenstream >> token){
			cout << token << " " << endl;
		}
		*/
		/////////////////////////////////////////////////////////////////////
		
				
		if(command != "quit" && command != "env" && command != "date" && command != "curr" && command!= "cd"){
			cout << "ERROR - '" << command << "' is an invalid command" << endl;
			continue; // DOES NOT BREAK THE PROGRAM. CONTINUE ACCEPTING COMMANDS
		}
		
		
		if(command == "quit"){
			if(tokenstream >> token){
				cout << "Received too many arguments for command '" << command << "'" << endl;
				continue;
			}
			else{
				// quit the program
				// the only time the program quits, is if the user enters 'quit' command
				// with NO FOLLOWING TOKENS
				return 0;
			}
		}
		
		
		if(command == "env"){
			if(tokenstream >> token){
				cout << "Received too many arguments for command '" << command << "'" << endl;
				continue;
			}
			else{
				char** var;
				for (var = variables; *var != 0; var++){
				char* currentVar = *var;
				cout << currentVar << endl;
				}
			}
		}
		
		
		if(command == "date"){
			if(tokenstream >> token){
				cout << "Received too many arguments for command '" << command << "'" << endl;
				continue;
			}
			
			else{
				time_t timebuf;
				time(&timebuf);
				cout << ctime(&timebuf);
			}
		}


		if(command == "curr"){
			if(tokenstream >> token){
				cout << "Received too many arguments for command '" << command << "'" << endl;
				continue;
			}
			
			else{
				char *buf;
				buf = get_current_dir_name();
				cout << buf << endl;
			}
		}
		
		
		if(command == "cd"){
			// must edit this to display an error message when too many tokens are 
			// entered by the user
			
			
			//cout << "BEFORE CHANGING DIRECTORY : " << endl;
			//system("pwd"); ///////////////////
			
			string newdir;
			
			// if there is one token following 'cd' command, we will change to the specify directory
			if((tokenstream >> newdir)){ 
				
				// if there are MORE than ONE tokens following the 'cd' command, print error message
				if(tokenstream >> token){
					cout << "Received too many arguments for command '" << command << "'" << endl;
					continue;
				}
				
				else{ //
					// change directory to a directory existing within the current directory
					int rc = chdir(newdir.c_str());
					
					// if the requested directory is NOT FOUND, we check if the root path is valid
					if(rc < 0){
						cout << "ERROR: Unable to locate directory" << endl;
						
						// save for when using 'meta character shortcuts'
						//if(newdir[0] == '~' && newdir[1] == '/') cout << "found ~/" << endl;
						
						//string rootpath(newdir.begin() + 2, newdir.end());
						//cout << "ROOTPATH: " << *(getenv("HOME")) + rootpath << endl;
						
						/*
						string rootpath;
						char *rootpath_p;				// use both of
						rootpath_p = getenv("HOME");	// these lines for root path directory access
						rootpath = *rootpath_p;
						cout << "rootpath: " << rootpath << endl;
								//rootpath = newdir;
						//cout << "rootpath (after adding newdir): " << rootpath << endl;
						if(chdir(rootpath.c_str()) < 0){
							cout << "ERROR: Unable to locate directory" << endl;
							continue;
						}
						*/
					}
				}
			}
			
			else{
				chdir(getenv("HOME"));
			}

		}
		
					
	} // end of outside while-loop
	
	return 0;
	
}
