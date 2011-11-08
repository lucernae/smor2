
#ifndef ROMS_MENU_H_
#define ROMS_MENU_H_

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
//other types
#include "Category.h"
#include "Menu_Item.h"
#include "Recipe.h"
#include "Ingredient.h"
#include "Order.h"
#include "Order_Item.h"

using namespace std;

namespace ROMS{   
	
	enum Msg_type{
	Exit_program=1, About_info, About_help, Close_about_box,
	Read_recipes_ingredients, Read_orders, Read_category_menu_items,
	Show_order, Show_recipe, Show_menu, 
	Show_categories, Show_order_items, // RMN B.3
	Find_category_sales, Find_table_sales, Find_menu_item_sales,
	Update_add_order_item, Update_add_menu_item, Update_add_recipe,
	Tables_button, Orders_button, Categories_button, Recipes_button, Menu_items_button,
	Display_window, Menu_bar_cb
	};
   
	struct InvalidType : std::runtime_error {
	  InvalidType(): runtime_error("Invalid type data") {}
	};

class ROMS_Menu {
public:
   struct InvalidData : std::runtime_error {
      InvalidData(): runtime_error("Invalid input data format") {}
   };

   struct InvalidFile : std::runtime_error {
      InvalidFile(): runtime_error("Invalid input file name") {}
   };

   enum Main_menu_options{ Info=1, Read, Show, Find, Update, Exit }; 

   enum Rec_type{ Cats=1, Menu_entry, Rec, Ingred, Ord, Ord_item };

   enum File_type{	Tbls=Tables_button, 
					CatMI=Categories_button, 
					Prds=Menu_items_button, 
					Carts=Orders_button, 
					Items=Recipes_button }; 
		
	ROMS_Menu();
   
   void display_main_menu() const;
   void display_info() const;//display assignment info & your name plus "Project Part I"
   void read();		 
   void update();
   void show() const;
   void find() const;
   void read_recipes_ingredients(string);
   void Read_orders(string);
   void read_catprds(string);
   	
   //show functions added to support GUI interface
   string show_button(Msg_type);

	template <typename T>
	string show_func(T t, string s)
	{
		string r;
		if(0==t.size())
			r = "No "+s+" Data Loaded\n";
		else
			if(s=="Category")
			{
				r+=Category::display_headers();
				r+="\n";
				for(int i=0; i<categories.size(); ++i)
				{
					r+=categories[i].display();
					r+="\n";
					// RMN B3 added special cases required for categories
					// displaying menu item names for each categories
					// a not-so-efficient loop, but this will do...
					r+="Menu Item Name\n";
					for(int j=0;j<menu_items.size();j++)
					{
						if(menu_items[j].get_cat_id()==categories[i].get_cat_id())
						{
							r+="\t"+menu_items[j].get_name();
							r+="\n";
						}
					}
					r+="\n";
				}
			}
			else if(s=="Order Items")
			{
				// RMN We had to rewrite the code, unfortunately....
				// The spec wants to group order items by its order...
				// we often wants to view the latest order, so iterate from the end
				for(int i=orders.size()-1; i>=0; --i)
				{
					// we write the output for each order
					// search all the order item with a not-so-efficient loop
					r+=Order::display_headers()+"\n";
					r+=orders[i].display();
					r+="\n";
					r+=Order_Item::display_headers()+"\n";
					for(int j=0;j<order_items.size(); j++)
					{
						if(orders[i].get_order_id()==order_items[j].get_order_id())
						{
							// print the order item
							r+=order_items[j].display();
							r+="\n";
						}
					}
					r+="\n";
				}
			}
			else
				for(int i=0; i<t.size(); ++i)
				{
					r+=t[i].display();
					r+="\n";
				}
		return r;
	}

	template <typename T>
	string show_tabular_func(T t, string s)
	{
		string r;
		if(0==t.size())
			r = "No "+s+" Data Loaded\n";
		else {
			r += t[0].display_headers() + "\n";
			for(int i=0; i<t.size(); ++i)
			{
				r+=t[i].display();
				r+="\n";
			}
		}
		return r;
	}
private:
   //private data
	vector<Category> categories;
	vector<Menu_Item> menu_items;
	vector<Recipe> recipes;
	vector<Ingredient> ingredients;
	vector<Order> orders;
	vector<Order_Item> order_items;


	int get_file_info(ifstream& ist, string prompt, string fname);	//A.1,2,3 merge added a function for common code
	void read_file(ifstream& file, int num_rec, Rec_type ftype);
	string get_long_string(ifstream& ist);
};
} //end namespace ROMS

#endif //ROMS_MENU_H_
