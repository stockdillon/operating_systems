/*
Dillon Stock stockdil@basset
2017-04-18
reverse_string.cpp

Your Comments
*/

#include <iostream>
#include <string.h>
#include <string>
#include <sstream>
using std::cout; using std::endl;
using namespace std;

int main(){
	string test;
	cin >> test;
	std::stringstream stream;
	stream << test;
	string front, back;
	while(stream >> front){
		stream >> back;
	}
	cout << front + back << endl;
	
}

