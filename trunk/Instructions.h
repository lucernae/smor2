
#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include <string>
#include <vector>
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

	void get_instructions_vector(vector<string>& out) const {
		size_t start = 0;
		size_t found = instructions.find("\n", start);

		while(found != string::npos) {
			out.push_back(instructions.substr(start, found-start));
			start = found + 1;
			found = instructions.find("\n", start);
		}

		out.push_back(instructions.substr(start));
	}

private:
   //constants
     
   // private data
	string instructions;
};


#endif //INSTRUCTIONS_H_
