/*
Dillon Stock stockdil@basset
2017-03-13
proj06.student.c

Your Comments
*/

#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
using std::cout; using std::endl;
using namespace std;

/*
void display_table(const vector<string> &table){
	for(unsigned i=0; i < table.size(); i++){
		cout << table[i] << endl;
	}
}
*/









struct pageTable{
	bool Vbits[16] = {};
	bool Rbits[16] = {};
	bool Mbits[16] = {};
	int logical_addresses[16] = {};
	//int virtual_addresses[16] = {};
};





void display_table(const struct pageTable &page_table){
	cout << endl; // begin with blank line...
	cout << "I V R M Fr" << endl;
	
	for(unsigned i = 0; i<16; i++){
		cout<< i << " " 
			<< page_table.Vbits[i] << " "
			<< page_table.Rbits[i] << " " 
			<< page_table.Mbits[i] << " " 
			<< std::hex << setw(2) << setfill('0') << page_table.logical_addresses[i]
			//<< page_table.virtual_addresses[i] << " " 
			//<< page_table.logical_addresses[i] << " " 
			<< endl;
	}
	cout << endl; // end with blank line...
}





int main(int argc, char *argv[], char** variables){
	
	if(argc != 3){
		cout << "Invalid number of arguments (expected 3)... quitting program" << endl;
		return 0;
	}
	
	
	// 'page_table' will consist of the lines of the page table, the mapping between virtual and physical addresses
	struct pageTable page_table;
	
	// get infilename, the name of the file which contains the memory reference instructions
	string infilename;
	infilename = string(argv[2]);
	//cout << "infilename: " << infilename << endl; // to display... REMOVE
	
	// get N, the number of commands after which the page table will be displayed
	int N;
	unsigned command_count = 0;
	unsigned R_count = 0;
	unsigned W_count = 0; 
	string N_error_test = argv[1];
	for(char c : N_error_test){
		if( c - '0' < 0 || c - '0' > 9){
			cout << "Invalid second argument (expected a positive integer or 0)... quitting program" << endl;
			return 0;
		}
	}
	N = stoi(argv[1], nullptr, 10);
	//cout << "N: " << N << endl; // to display..... REMOVE
	
	
	if(N > 0) display_table(page_table);
	
	// initialize ifstream buffer which will retrieve the contents of the input file "infilename"
	ifstream instream(infilename.c_str());
	if(!instream){
		cout << "Invalid filename... quitting program" << endl;
		return 0;
	}
	
	// "temp_line" will temporarily hold each line of the input file
	string temp_line;
	while(getline(instream, temp_line)){
		
		//char op;
		string op;
		string mem_ref_str;
		int mem_ref_int;
		
		
		op.assign(temp_line,0,1); // assigns a substring starting at temp_line index 0, of length 1, to 'op'
		// Increment the appropriate Read/Write operation count
		if(op == "W") W_count++;
		if(op == "R") R_count++;
		
		mem_ref_str.assign(temp_line,2,4);
		//cout << "mem_ref_str: " << mem_ref_str << endl;
		mem_ref_int = stoi(mem_ref_str, nullptr, 16);
		//cout << "mem_ref_int: " << std::hex << mem_ref_int << endl;
		
		unsigned page_num = ( (mem_ref_int << 16) >> 28) & 0x0000000f;
		//cout << "PAGE NUM: " << std::hex << page_num << endl;
		unsigned page_offset = ( (mem_ref_int << 20) >> 20 ) & 0x00000fff;
		//cout << "PAGE OFFSET: " << std::hex << page_offset << endl;
		
		cout << op << " " << mem_ref_str << " " << std::hex << page_num << " " << setw(3) << setfill('0') << page_offset << endl;
		
		// increment command_count,
		// if we have performed N commands, we must display the page table
		command_count++;
		if(N > 0 && command_count % N == 0){
			display_table(page_table);
			cout << endl;
		}
		
		//cout << endl << endl;
		
	}
		
	if(N > 0 && command_count % N != 0){
		display_table(page_table);
	}
	
	cout << "Total number of memory references: " << command_count << endl;
	cout << "Total number of read operations: " << R_count << endl;
	cout << "Total number of write operations: " << W_count << endl;
	

}

