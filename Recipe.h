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
        Recipe () {}
	Recipe (int id, string n, Instructions a)	//uncomment once you have added the type definition
		: recipe_id(id), chef_name(n), instr(a) {}

   // utility functions
	//## Ecky B2
	static string display_headers() {
		ostringstream oss;
		oss		<< left << setw(20) << "Chef Name"
				<< "Instructions";
		return oss.str();
	}

	//##Ecky B2
	string display() const {	//B.1.a
		vector<string> s;
		instr.get_instructions_vector(s);
		
		ostringstream oss;
		oss		<< left << setw(20) << chef_name
				<< s[0] << endl;
		for(int i = 1; i < s.size(); ++i) {
			oss	<< left << setw(20) << " "
				<< s[i] << endl;
		}
		return oss.str();
	}	//B.1.a

	int get_rec_id() const {return recipe_id;}	//B.1.a
	string get_chef()const {return chef_name;}	//B.1.b
  Instructions instruc() const {return instr;} //RCD B1

  //RCD C.3
  friend istream& operator>>(istream& stream, Recipe& r)
  {
    int id;
    string chef, ins;
    stream >> id >> chef >> ins;
    if(!stream) {
      return stream;
    }
    r = Recipe(id, chef, Instructions(ins));
    return stream;
  }
private:
   //constants
     
   // private data
	int recipe_id;
	string chef_name;
	Instructions instr;			//uncomment once you have added the type definition
};

struct SortRecipesByChef		//B.1.b
{
   bool operator()(const Recipe& a,const Recipe& b) const
   {
	   return a.get_chef() < b.get_chef();
   }
};
#endif //RECIPE_H_
