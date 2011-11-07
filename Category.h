/*
	The directives starting with # below ensures that this file is read by the compiler only once
	even if it is #included several times. It is call an "include guard"
*/
#ifndef CATEGORY_H_
#define CATEGORY_H_

#include <string>
#include <sstream>
using namespace std;

struct Category {
public:
	// constructors
	Category () {}
	Category (int id, string n)
		:cat_id(id), cat_name(n) {}
    // utility functions
	string display() const {	
		ostringstream oss;
		oss << cat_id << " " << cat_name ;
		return oss.str();
	}

private:
   //constants
     
   // private data
	int cat_id;
	string cat_name;
};


#endif //CATEGORY_H_
