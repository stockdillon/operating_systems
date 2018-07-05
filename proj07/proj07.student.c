/*
Dillon Stock stockdil@basset
2017-03-13
proj07.student.c

Your Comments
*/

#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <queue>
#include <algorithm>
using std::cout; using std::endl;
using namespace std;


struct pageTable{
	bool Vbits[16] = {};
	bool Rbits[16] = {};
	bool Mbits[16] = {};
	unsigned frames[16] = {};
	//int logical_addresses[16] = {};
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
			<< std::hex << setw(2) << setfill('0') << page_table.frames[i]
			//<< std::hex << setw(2) << setfill('0') << page_table.logical_addresses[i]
			//<< page_table.virtual_addresses[i] << " " 
			//<< page_table.logical_addresses[i] << " " 
			<< endl;
	}
	cout << endl; // end with blank line...
}





int main(int argc, char *argv[], char** variables)
{
	
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
	
	if(N > 0) display_table(page_table);
	
	// initialize ifstream buffer which will retrieve the contents of the input file "infilename"
	ifstream instream(infilename.c_str());
	if(!instream){
		cout << "Invalid filename... quitting program" << endl;
		return 0;
	}
	
	
	
	//////////////////////////////////////// NEW Variables /////////////////////
	// general variables
	string repAlg;
	string temp_falloc;
	unsigned falloc;
	getline(instream, repAlg);
	getline(instream, temp_falloc);
	falloc = stoi(temp_falloc, nullptr, 10);
	//char WB = ' ';
	unsigned faults = 0;
	//cout << "replacement algorithm: " << repAlg << endl; // remove
	//cout << "frames allocated: " << std::dec << falloc << endl; // remove
	if(falloc < 1 || falloc > 16){
		cout << "Error in file, frame allocation must be between 1 and 16 (inclusive)" << endl;
		return -1;
	}
	/////////////////////////////////
	
	
	// variables for FIFO
	vector<int> fifo_q;
	
	
	// variables for LRU
	//int time_since_used[16];
	//std::fill_n(time_since_used, 16, -1);
	vector<int> lru_q;
	
	// variables for Clock
	unsigned clockBuf = 0;
	unsigned clockPageCount = 0;
	int clock_arr[falloc];
	
	//////////////////////////////////////// NEW Variables /////////////////////
	
	
	
	
	
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
		
		
		
		
		
		
		
		
		
		
		
		

		////////////////////////////////////////////////////////////////////////
		/////////////////////////////// FIFO ///////////////////////////////////
		////////////////////////////////////////////////////////////////////////
		//if(repAlg == "FIFO"){
		if(repAlg != "LRU" && repAlg != "Clock"){
			// Defaults to FIFO replacement algorithm
			
			// for page faults...
			if(find(fifo_q.begin(), fifo_q.end(), page_num) == fifo_q.end()){
				//cout << "page NOT FOUND! (page fault within FIFO)" << endl;
				page_table.frames[page_num] = 0x40 + (faults % falloc);
				faults++;
				
				//if(op == "W"){
				if(fifo_q.size() == falloc && page_table.Mbits[fifo_q[0]] == 1){
					cout << op << " " << mem_ref_str << " " << std::hex << page_num << " " 
					<< setw(3) << setfill('0') << page_offset << " " << "F WB " 
					<< setw(2) << page_table.frames[page_num] << setw(3) << page_offset << endl;
					//page_table.Mbits[page_num] = 1;
				}
				else{
					cout << op << " " << mem_ref_str << " " << std::hex << page_num << " " 
					<< setw(3) << setfill('0') << page_offset << " F    " 
					<< setw(2) << page_table.frames[page_num] << setw(3) << page_offset << endl;
					page_table.Mbits[page_num] = 0;
				}
				if(fifo_q.size() >= falloc){
					page_table.Vbits[fifo_q[0]] = 0; // ******* reset victim page control bits to 0
					page_table.Rbits[fifo_q[0]] = 0; // **
					page_table.Mbits[fifo_q[0]] = 0; // **
					fifo_q.erase(fifo_q.begin());
				}
				fifo_q.push_back(page_num);
				page_table.Rbits[page_num] = 1;
				page_table.Vbits[page_num] = 1;
				if(op == "W") page_table.Mbits[page_num] = 1;
			}
			
			// for pages that are currently in the page table
			else{
				/*
				if(op == "W"){
					cout << op << " " << mem_ref_str << " " << std::hex << page_num << " " 
					<< setw(3) << setfill('0') << page_offset << "   WB " 
					<< setw(2) << page_table.frames[page_num] << setw(3) << page_offset << endl;
				}
				*/
				
				//else{
					cout << op << " " << mem_ref_str << " " << std::hex << page_num << " " 
					<< setw(3) << setfill('0') << page_offset << "      " 
					<< setw(2) << page_table.frames[page_num] << setw(3) << page_offset << endl;
				//}
				page_table.Rbits[page_num] = 1;
				if(op == "W") page_table.Mbits[page_num] = 1;				
			}
		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		////////////////////////////////////////////////////////////////////////
		/////////////////////////////// LRU ////////////////////////////////////
		////////////////////////////////////////////////////////////////////////
		else if(repAlg == "LRU"){
			bool fault = false;
			bool WB = false;
			if(find(lru_q.begin(), lru_q.end(), page_num) == lru_q.end()) fault = true;
			if(fault){
				if(lru_q.size() == falloc && page_table.Mbits[lru_q[0]] == 1){
					WB = true;
				}
			}
			
			//if(find(lru_q.begin(), lru_q.end(), page_num) != lru_q.end()){
			if(!fault){
				// this PREVENTS DUPLICATES... each page number can only be in the queue once
				lru_q.erase(find(lru_q.begin(), lru_q.end(), page_num));
			}
			
			else{ // when page fault occurs...
				if(lru_q.size() < falloc){			// *** ??? ***
					page_table.frames[page_num] = 0x40 + (faults % falloc);
				}
				else{
					page_table.frames[page_num] = page_table.frames[lru_q[0]];
				}
				faults++;
				if(lru_q.size() >= falloc){
					page_table.Vbits[lru_q[0]] = 0; //******** reset victim page control bits to 0
					page_table.Rbits[lru_q[0]] = 0; //**
					page_table.Mbits[lru_q[0]] = 0; //**
					lru_q.erase(lru_q.begin());
				}	
			}
			
			lru_q.push_back(page_num);

			page_table.Rbits[page_num] = true;
			page_table.Vbits[page_num] = 1;
			if(op == "W") page_table.Mbits[page_num] = 1;
			else if(fault) page_table.Mbits[page_num] = 0;
			
			
			if(fault){ 
				if(WB){
					cout << op << " " << mem_ref_str << " " << std::hex << page_num << " " 
					<< setw(3) << setfill('0') << page_offset << " " << "F WB " 
					<< setw(2) << page_table.frames[page_num] << setw(3) << page_offset << endl;
				}
				
				else{
					cout << op << " " << mem_ref_str << " " << std::hex << page_num << " " 
					<< setw(3) << setfill('0') << page_offset << " F    " 
					<< setw(2) << page_table.frames[page_num] << setw(3) << page_offset << endl;
				}
			}
			
			
			else{
				cout << op << " " << mem_ref_str << " " << std::hex << page_num << " " 
				<< setw(3) << setfill('0') << page_offset << "      " 
				<< setw(2) << page_table.frames[page_num] << setw(3) << page_offset << endl;
			}
		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		////////////////////////////////////////////////////////////////////////
		/////////////////////////////// CLOCK //////////////////////////////////
		////////////////////////////////////////////////////////////////////////
		else if(repAlg == "Clock"){
			bool fault = false;
			bool WB = false;
			if(page_table.Vbits[page_num] != 1){
				bool found = false;
				fault = true;
				
				if(clockPageCount < falloc){
					page_table.Vbits[page_num] = 1;
					page_table.Rbits[page_num] = 1;
					if(op == "W") page_table.Mbits[page_num] = 1;
					page_table.frames[page_num] = 0x40 + (faults % falloc);
					//clock_q.push_back(page_num); // ***
					clock_arr[clockPageCount] = page_num; // ***
				}
								
				else{
					/*
					cout << "displaying clock_arr..." << endl;
					for(unsigned i=0; i<falloc; i++){
						//cout << i << " | " << clock_arr[i] << " (" << page_table.Rbits[clock_arr[clockBuf % falloc]] << ")";
						cout << i << " | " << clock_arr[i] << " (" << page_table.Rbits[clock_arr[i]] << ")";						
						if(clockBuf % falloc == i) cout << "<- ";
						cout << endl;
					}
					*/
					
					while(!found){
						//page_table.Rbits[clockBuf % 16] = 0;
						//if(page_table.Vbits[clockBuf % 16] == 1 && page_table.Rbits[clockBuf % 16] == 0){
						if(page_table.Rbits[clock_arr[clockBuf % falloc]] == 0){
							//cout << "found victim... page " << clock_arr[clockBuf % falloc] << endl;
							//cout << "index: " << (clockBuf % falloc) << " of clock_arr" << endl;
							found = true;
							page_table.Vbits[clock_arr[clockBuf % falloc]] = 0; // reset victim control bits to 0
							page_table.Rbits[clock_arr[clockBuf % falloc]] = 0; // **
							page_table.Mbits[clock_arr[clockBuf % falloc]] = 0; // **
							if(page_table.Mbits[clock_arr[clockBuf % falloc]] == 1) WB = true; // ****
							if(op == "W") page_table.Mbits[page_num] = 1;
							page_table.frames[page_num] = page_table.frames[clock_arr[clockBuf % falloc]];
							page_table.Vbits[clock_arr[clockBuf % falloc]] = 0;
							clock_arr[clockBuf % falloc] = page_num;
							
						}
						if(!found) page_table.Rbits[clock_arr[clockBuf % falloc]] = 0;
						clockBuf++;
						/*
						cout << "displaying clock_arr..." << endl;
						for(unsigned i=0; i<falloc; i++){
							//cout << i << " | " << clock_arr[i] << " (" << page_table.Rbits[clock_arr[clockBuf % falloc]] << ")";
							cout << i << " | " << clock_arr[i] << " (" << page_table.Rbits[clock_arr[i]] << ")";
							if(clockBuf % falloc == i) cout << "<- ";
							cout << endl;
						}
						*/
					}
				}
				faults++;
				clockPageCount++;
			}

			page_table.Vbits[page_num] = 1;
			page_table.Rbits[page_num] = 1;
			if(op == "W") page_table.Mbits[page_num] = 1;
			//else if(fault) page_table.Mbits[page_num] = 0;
			
			if(fault){
				if(WB){ 
					cout << op << " " << mem_ref_str << " " << std::hex << page_num << " " 
					<< setw(3) << setfill('0') << page_offset << " " << "F WB " 
					<< setw(2) << page_table.frames[page_num] << setw(3) << page_offset << endl;
				}
				
				else{
					cout << op << " " << mem_ref_str << " " << std::hex << page_num << " " 
					<< setw(3) << setfill('0') << page_offset << " F    " 
					<< setw(2) << page_table.frames[page_num] << setw(3) << page_offset << endl;
				}
			}
			
			else{
				cout << op << " " << mem_ref_str << " " << std::hex << page_num << " " 
				<< setw(3) << setfill('0') << page_offset << "      " 
				<< setw(2) << page_table.frames[page_num] << setw(3) << page_offset << endl;
			}
		}
		
		
		
		
		
		
		
		
		
		
		
		
		/////////////////////////////// Error //////////////////////////////////

		else{
			cout << "Invalid string given for replacement algorithm... quitting program" << endl;
			return -1;
		}
		////////////////////////////////////////////////////////////////////////
		
		
		
		
		
		// increment command_count,
		// if we have performed N commands, we must display the page table
		command_count++;
		if(N > 0 && command_count % N == 0){
			display_table(page_table);
			//cout << endl;
		}
		
		//cout << endl << endl;
		
	}
		
	if(N > 0 && command_count % N != 0){
		display_table(page_table);
	}
	
	cout << std::dec;
	if(repAlg == "FIFO" || repAlg == "LRU" || repAlg == "Clock"){
		cout << "Page replacement algorithm used: " << repAlg << endl;
	}
	else{ cout << "Page replacement algorithm used: " << "FIFO (by default)" << endl; }
	cout << "Number of frames allocated: " << falloc << endl;
	
	cout << "Total number of memory references: " << command_count << endl;
	cout << "Total number of read operations: " << R_count << endl;
	cout << "Total number of write operations: " << W_count << endl;
	

}

