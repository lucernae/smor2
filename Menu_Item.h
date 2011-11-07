#ifndef MENU_ITEM_H_
#define MENU_ITEM_H_

#include <string>
#include <iomanip>

// other types
#include "Description.h"				//uncomment once you have added the type definition
using namespace std;

struct Menu_Item
{
public:
	// constructors
	Menu_Item (int id, int cat, int rec, string n, double amt, Description s)	//uncomment once you have added the type definition
		: menu_item_id(id), cat_id(cat), recipe_id(rec), menu_item_name(n), price(amt), descr(s) {}

	// utility functions
	//## Ecky B2
	static string display_headers() {
		ostringstream oss;
		oss		<< left << setw(10) << "ID"
				<< left << setw(30)	<< "Name"
				<< left << setw(10) << "Price"
				<< "Description";
		return oss.str();
	}

	//## Ecky B2
	string display() const {
		ostringstream oss;
		oss		<< left << setw(10) << menu_item_id
				<< left << setw(30) << menu_item_name
				<< left << setw(10) << price
				<< descr.display();
		return oss.str();
	}	

	string get_name() const {return menu_item_name;} //B.1.a
	int get_recipe_id() const {return recipe_id;} //B.1.a
	string get_description() const {return descr.display();}	//B.1.a
	int get_menu_item_id() const {return menu_item_id;}	//B.2.a
	double get_price() const {return price;}	//B.2.a
private:
   //constants
     
   // private data
	int menu_item_id;
	int cat_id;
	int recipe_id;
	string menu_item_name;
	double price;
	Description descr;			//uncomment once you have added the type definition
   
};


#endif //MENU_ITEM_H_
