#ifndef RECIPE_H_
#define RECIPE_H_

#include <string>
#include <sstream>			//B.1.a

// other types
#include "Instructions.h"				//uncomment once you have added the type definition
using namespace std;
 
struct Recipe {
public:
	// constructors
	Recipe (int id, string n, Instructions a)	//uncomment once you have added the type definition
		: recipe_id(id), chef_name(n), instr(a) {}

   // utility functions
	string display() const {	//B.1.a
		ostringstream oss;
		oss << "Chef Name: " << chef_name << endl << instr.display() ;
		return oss.str();
	}	//B.1.a
	int get_rec_id() const {return recipe_id;}	//B.1.a
	string get_chef()const {return chef_name;}	//B.1.b
private:
   //constants
     
   // private data
	int recipe_id;
	string chef_name;
	Instructions instr;			//uncomment once you have added the type definition
};

struct SortRecipesByChef		//B.1.b
{
   bool operator()(Recipe& a, Recipe& b) const
   {
	   return a.get_chef() < b.get_chef();
   }
};
#endif //RECIPE_H_
