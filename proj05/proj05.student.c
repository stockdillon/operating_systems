#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>
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
#include <cstring>
#include <sys/wait.h>
#include <sys/types.h>
#include <vector>
#include <algorithm>





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
	
	
	////////////////////////////////////////////////////
	// project 4 added variables
	string command_list[10];
	int command_count = -1;

	
	string dir_list[10];
	//int dir_count = -1;
	int dir_count = 0;
	
	string newdir;
	int dir_test;
	////////////////////////////////////////////////////
	
	
	
	
	
	
	
	////////////////////////////////////////////////////
	// project 5 added variables
	
	//int parent_pid;
	//int child_pid;
	int child_status = 0;
	vector<std::string> token_vect;
	vector<char*> token_ptr_vect;
	char* args_[128];
	
	vector<string> env_vars;
	char** var;
	for (var = variables; *var != 0; var++){
		char* currentVar = *var;
		//cout << currentVar << endl;
		env_vars.push_back((string)currentVar);
	}
	
	////////////////////////////////////////////////////
	
	
	
	
	

	
	
	
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	while(true){
		display_prompt( count, id_string);
		count++;
		getline(cin, tokens);
		////////////////////////////////////////////////////
		while(tokens[0] == ' ' || tokens[0] == '\t') tokens.erase(tokens.begin());
		if(tokens[0] == '!'){
			// for COMMAND: "N!"
			/////////////////////////////////////////////////////////////////////////////
				tokens.erase(tokens.begin());
				bool valid = true;
				command = ""; //////////////////
				for(char c : tokens){
					if(c - '0' > 9 || c - '0' < 0){
						cout << "Invalid command index (expected single integer)" << endl;
						valid = false;
						break;
					}
					if( c == ' ' || c == '\t') break;
					command += c;
				}
				if(!valid){
					count--;
					continue;
				}		
				int command_num = atoi(command.c_str());
				
				//////////////////////////////////////////////////////
				// make sure command num is BELOW COMMAND COUNT and ABOVE (COMMAND COUNT - 10)
				if(command_num < 1){
					cout << "Invalid command index (expected integer greater than 0)" << endl;
					count--;
					continue;
				}
				if(command_num > command_count+1){
					cout << "Invalid command index (too high)" << endl;
					count--;
					continue;
				}
				if(command_num < (command_count - 8)){
					cout << "Invalid command index (too low)" << endl;
					count--;
					continue;
				}
				//////////////////////////////////////////////////////
			
				int steps_back;
				steps_back = command_count - command_num;
				//cout << setw(4) << right << command_count - steps_back  << left << "   " << command_list[(command_count-steps_back-1)%10] << endl;
				cout << command_list[(command_count-steps_back-1)%10] << endl;
				
				tokens = command_list[(command_count-steps_back-1)%10];
		}
		/////////////////////////////////////////////////////////////////////////////
		
		istringstream tokenstream(tokens);
		command_count++;
		std::stringstream command_and_tokens(tokenstream.str());
		///////////////////////////////////////////////////////
		
		
		
		

		
		
		
				
		// if 'tokenstream' is empty, there is no command; continue prompting user for command
		// the FIRST TOKEN in the line (tokenstream) is the COMMAND
		if(!(tokenstream >> command)){
			// if the user enters an empty line, we do NOT increment the command count
			// since the count increments by default, we must decrement it
			count--;
			command_count--;
			continue;
		}
		
		
		
		

		
		
		

		
		//////////////////////////////////////////
		// builds command list for displaying when user enters command 'clist'
		// each line of command + tokens is appended to clist, separated by a single space.
		string temp_token;
		string temp_line;
		int arg_i = 0;
		token_vect.clear(); //*********************
		while(command_and_tokens >> temp_token){
			temp_line += temp_token + " ";
			token_vect.push_back(temp_token); //********************
			args_[arg_i] = (char*)temp_token.c_str(); //*********
			arg_i++; //*************
		}
		command_list[command_count%10] = temp_line;
		//////////////////////////////////////////
		
		
		
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
				
		if(command != "quit" && command != "env" && command != "date" && command != "curr" 
		&& command!= "cd" && command != "clist" && command != "dlist"){
			//cout << "ERROR - '" << command << "' is an invalid command" << endl;
			//////////////////////////////////////////////////////////////////////////////////////
			//cout << getpid() << endl;
			//parent_pid = getpid();
			
			int pid; //child_pid;
			//int child_status;
			
			//int status, child_status;
			/*
			for(unsigned i=0; i<token_vect.size(); i++){
				cout << token_vect[i] << endl;
				cout << "args_[" << i << "]: " << args_[i] << endl;
			}
			*/
			args_[token_vect.size()] = NULL; // move this to appropriate line ****
			
			


			pid = fork();

			if (pid > 0){
				child_status = 0; //****
				//printf( "Entering parent code -- PID: %d  PPID: %d\n\n", getpid(), getppid() );
				
				// & token found, must wait for child process
				if(string(args_[token_vect.size() - 1]) != "&"){
					wait(&child_status);
					//child_pid = wait( &child_status );
				}
				
				
				
				// & not found, continue with parent processing

			//printf( "Child %d terminated with status %d\n\n", child_pid, WEXITSTATUS( child_status ) );

			//printf( "Exiting parent code  -- PID: %d  PPID: %d\n\n", getpid(), getppid() );

			//status = 0;
			continue; // parent process keeps accepting commands... 
			}
			
			
			else if (pid == 0){
				//printf( "Entering child code  -- PID: %d  PPID: %d\n\n", getpid(), getppid() );
				
				if(string(args_[token_vect.size() - 1]) == "&"){
					args_[token_vect.size() - 1] = NULL;
					if(token_vect.size() == 1){
						child_status = -1;
						exit(child_status);
					}
				}
				
				/*
				if(-1 != execv(args_[0], args_)){
					//cout << "execv FAILED" << endl;
				}
				*/
				
								
				//else {
					//display_prompt( count, id_string);
					if(-1 == execvp(args_[0], args_)){
						//cout << "execvp FAILED" << endl;
					}
				//}

				cout << "External command not found" << endl;
				//display_prompt(count, id_string);
				//printf( "Exiting child code   -- PID: %d  PPID: %d\n\n", getpid(), getppid() );

				child_status = -1;
				exit( child_status ); // child process terminates (failed command)
			}
			
						
			
			
			
			
			/*
			// fork...
			if (child_pid = fork ()){ // code executed by parent
				printf ("This is the parent, child pid is %d\n", child_pid);
				int status;
				pid_t w = waitpid(child_pid, &status, WCONTINUED);
				if(w == -1){
					cout << "Error (w)" << endl;
				}
				execl("./test", "./test");
				system("mkdir test2");
				execl("ls", "ls");
				continue;
			}
			
			else { // code executed by child
				printf ("This is the child, pid is %d\n",getpid ());
				execl("test", "test");
				//pause();
				cout << "parent PID: " << getppid() << endl;
				_exit(atoi(argv[1]));
				cout << "did not exit" << endl;
				return 0;
			}
			// if pid == parent process pid... 
			// 		then continue.
			// if pid == child process pid...
			// 		then execl( filename, args, nullptr)
			//		AND THEN KILL THIS PROCESS!
			*/ 
			
			/*
			if(getpid() == child_pid){
				execl("test", "test");
			}
			*/
			///////////////////////////////////////////////////////////////////////////////////////
			return 0; 
		}
		
		
		
		
		
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		
		
		
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
		
		
		
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		
		if(command == "env"){
			string temp_var;
			// token becomes the name of environment variable that is to be set or displayed
			if(tokenstream >> token){
				string token2;
				// token2 becomes the new value of whichever environment variable is being changed
				if(tokenstream >> token2){
					string too_many_args;
					if(tokenstream >> too_many_args){
						cout << "Received too many arguments for command '" << command << "'" << endl;
						continue;
					}
					
					if(getenv(token.c_str()) == nullptr){
						temp_var = token + "=" + token2;
						env_vars.push_back(temp_var); // *****
					}
					
					else{
						temp_var = token + "=" + (string)getenv(token.c_str());
						auto it = std::find(env_vars.begin(), env_vars.end(), temp_var);
						if(it != env_vars.end())
							env_vars.erase(it);
						string temp_var = token + "=" + token2;
						env_vars.push_back(temp_var); // *****
					}
					setenv(token.c_str(), token2.c_str(), 1);
					continue;
					// continue WITHOUT PRINTING the new value of the environment variable that was changed
				}
				
				if(getenv(token.c_str()) != nullptr){
					cout << getenv(token.c_str()) << endl;
				}
				else{
					cout << "Environment variable not found" << endl;
				}
			}
			else{
				for(unsigned i = 0; i < env_vars.size(); i++){
					cout << env_vars[i] << endl;
				}
				/*
				vector<string> env_vars;
				char** var;
				for (var = variables; *var != 0; var++){
					char* currentVar = *var;
					cout << currentVar << endl;
					env_vars.push_back((string)currentVar);
				}
				*/
			}
		}
		
		
		
		
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		
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






/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



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
		
		
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		
		
		if(command == "cd"){
			string prev_dir = get_current_dir_name();
			
			
			// if there is one token following 'cd' command, we will change to the specified directory
			if((tokenstream >> newdir)){
				
				// if there are MORE than ONE tokens following the 'cd' command, print error message
				if(tokenstream >> token){
					cout << "Received too many arguments for command '" << command << "'" << endl;
					continue;
				}
				
				
				else{ 
					if(newdir == "~"){
						//if(prev_dir != dir_list[dir_count%10]){ //
						if(prev_dir != get_current_dir_name()){ // ***************************
							dir_count++;
							dir_list[dir_count%10] = prev_dir;
							//cout << "added '" << prev_dir << "' to dir_list..." << endl;
						}
						chdir(getenv("HOME"));
						//chdir("/user");
						continue;
					}
					
					
					if(newdir[0] == '~'){
						newdir.erase(newdir.begin());
						string new_home_dir  = "/user/" + newdir;
						int test = chdir(new_home_dir.c_str());
						if(test < 0) cout << "Unknown user: " << newdir << endl;
						else{
							//if(prev_dir != dir_list[dir_count%10]){
							if(prev_dir != get_current_dir_name()){ // ***************************
								dir_count++;
								dir_list[dir_count%10] = prev_dir;
							}
						}
						continue;
					}
					
					
					
					
					
					
					// FOR COMMAND "CD #N"
					///////////////////////////////////////////////////////////////////////
					if(newdir[0] == '#'){
						newdir.erase(newdir.begin());
						bool valid = true;
						for(char c : newdir){
							if(c - '0' > 9 || c - '0' < 0){
								cout << "Invalid directory index (expected integer)" << endl;
								valid = false;
								break;
							}
						}
						if(!valid) continue;
						int dir_num = atoi(newdir.c_str());
						//cout << "directory INDEX: " << dir_num << endl;
						if(dir_num - 1 > dir_count || dir_num - 1 < dir_count - 10){
							cout << "Invalid directory index (select from 10 previous commands)" << endl;
							continue;
						}
						else{
							int steps_back;
							steps_back = dir_count - dir_num;
							//cout << setw(4) << right << dir_count - steps_back << left << "   " << dir_list[(dir_count-steps_back)%10] << endl;
							chdir(dir_list[(dir_count-steps_back)%10].c_str());
							// *************************************
							//if(prev_dir != dir_list[(dir_count-steps_back)%10].c_str()){
							if(prev_dir != get_current_dir_name()){ // ***************************
								dir_count++;
								dir_list[dir_count%10] = prev_dir;
							}
							// **************************************
							
							continue;
						}
					}
					////////////////////////////////////////////////////////////////////////
					
					
					
					
					
					
					// change directory to a directory existing within the current directory
					dir_test = chdir(newdir.c_str());
		
					// add the previous directory to the directory list, before switching to new directory
					/////////////////////////////////////////////////////////////////////////////////////
					if(dir_test >= 0){
						if(prev_dir != dir_list[dir_count%10]){
							dir_count++;
							dir_list[dir_count%10] = prev_dir;
						}
					}
					/////////////////////////////////////////////////////////////////////////////////////
					
					
					
					
					
					// if the requested directory is NOT FOUND, we check if the root path is valid
					if(dir_test < 0){
						cout << "ERROR: Unable to locate directory" << endl;
					}
				}
			}
			
		
			
			
			else{
				dir_test = chdir(getenv("HOME"));
				if(dir_test >= 0){
					//if(prev_dir != dir_list[dir_count%10]){
					if(prev_dir != get_current_dir_name()){ // ***************************
						dir_count++;
						dir_list[dir_count%10] = prev_dir;
					}
				}
			}
		} // end of 'cd' conditional

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



		
		
		if(command == "dlist"){
			if(tokenstream >> token){
				cout << "Received too many arguments for command '" << command << "'" << endl;
				continue;
			}
			
			else{
				int steps_back;
				(dir_count > 10) ? steps_back = 10 : steps_back = dir_count;
				for(int i = steps_back; i >0; i--){
					cout << setw(4) << right << dir_count - i + 1 << left << "   " << dir_list[(dir_count-i+1)%10] << endl;
				}
			}
			
			
		}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		
		if(command == "clist"){
			if(tokenstream >> token){
				cout << "Received too many arguments for command '" << command << "'" << endl;
				continue;
			}
			
			else{
				int steps_back;
				(command_count > 9) ? steps_back = 9 : steps_back = command_count;
				for(int i = steps_back; i >=0; i--){
					cout << setw(4) << right << command_count - i + 1 << left << "   " << command_list[(command_count-i)%10] << endl;
				}
			}
		}
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		
	} // end of outside while-loop
	
	return 0;
	
}
