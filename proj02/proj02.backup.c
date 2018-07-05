
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
//#include <boolalpha>
using std::cout; using std::endl; using std::cin;
using namespace std;

int main(int argc, char *argv[]){
	//char filename[50];
	//char contents[500]; //remove
	
	/*
	ifstream in;
	ofstream out;
	
	cout << "enter file name." << endl;
	cin >> filename;
	
	cout << "enter the contents of the new file." << endl;
	cin >> contents;
	//out << contents;
	
	in.open(filename);
	out.open(filename);
	out << contents << contents[1] << endl;
	out.close();
	*/
	char help_menu[] = "Enter a series of filenames with contents you would like to have displayed. \n"
	"Use the option '-t' to display the title of each file along with the contents of the file. \n"
	"Use the option '-n' to display line numbers along with the contents of the file. \n"
	"Use the option '-h' to display a help menu. \n";
		
	//char textblock[512];
	bool tset = false;
	bool nset = false;
	string temp;
	
	for(int i=1; i< argc; i++){		
		ifstream infile(argv[i]);
		//cout << "infile: " << infile << endl; //remove
		//cout << *(argv+i) << endl; //remove
		if(!infile.is_open()){
			//getline(infile, temp);
			//infile.getline(temp, 3);
			//cout << infile; //remove
			//infile >> temp; //remove
			
			temp = *(argv+i);
			if(temp != "-t" && temp != "-n" && temp != "-h"){
				cout << "Could not open file '" << temp << "'" << endl;
				continue;
			}
			//cout << "temp: " << temp << endl; //remove
			if(tset || temp == "-t"){
				tset = true;
				cout << "title option set" << endl; //remove?
				continue;
			}
			if(nset || temp == "-n"){
				nset = true;
				cout << "line numbering set" << endl;
				continue;
			}
			if(temp == "-h"){
				cout << "help menu selected..." << endl; //remove
				cout << help_menu;
				continue;
			}
		}
		
		
		if(tset){
			cout << std::string(strlen(*(argv+i)), ':') << endl;
			cout << *(argv+i) << endl;
			cout << std::string(strlen(*(argv+i)), ':') << endl;
		}
		
		char x;
		while(infile.get(x)) cout << x;
		cout << endl;
		//cout << "filename: " << filename << endl;
	}
	
	
	/*
	while(!feof(stdin)){
		cin >> filename;
		if(feof(stdin)) break;
		cout << filename << endl;
	}
	*/
	return 0;
}

