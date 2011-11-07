
#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include <string>
#include <sstream>			//B.1.a
using namespace std;

struct Instructions {
public:
	// constructors
	Instructions () {}
	Instructions (string s): instructions(s) {}
    // utility functions
	string display() const {	//B.1.a
		ostringstream oss;
		oss << instructions ;
		return oss.str();
	}	//B.1.a

private:
   //constants
     
   // private data
	string instructions;
};


#endif //INSTRUCTIONS_H_
