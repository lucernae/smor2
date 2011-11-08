/*
	The directives starting with # below ensures that this file is read by the compiler only once
	even if it is #included several times. It is call an "include guard"
*/
#ifndef CATEGORY_H_
#define CATEGORY_H_

#include <string>
#include <sstream>
#include <iomanip> // RMN add include
using namespace std;

struct Category {
public:
	// constructors
	Category () {}
	Category (int id, string n)
		:cat_id(id), cat_name(n) {}
    // utility functions
	// RMN added function
	static string display_headers(){
		ostringstream oss;
		oss		<< left << setw(12)<< "Category ID"
				<< "Category Name";
		return oss.str();
	}

	string display() const {	
		ostringstream oss;
		//oss << cat_id << " " << cat_name ;
		oss << left << setw(12) << cat_id << cat_name; // RMN add some formatting
		return oss.str();
	}

	int get_cat_id() const {return cat_id; } // RMN 

private:
   //constants
     
   // private data
	int cat_id;
	string cat_name;
};


#endif //CATEGORY_H_
