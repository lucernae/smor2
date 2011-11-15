//============================================================================
// Name        : ROMS_main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : C++ project Part II for your class (121, or 112) 
//============================================================================
//
#include "Simple_window.h"
#include "Graph.h"
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Text_Display.H>

#include "ROMS_Menu.h"

using namespace std;
using namespace ROMS;
using namespace Graph_lib;

//types we need for a ROMS GUI interface-- a memu_bar and a place to display text and a means of attaching them to a Window.

namespace Graph_lib {
//------------------------------------------------------------------------------
	struct Window_text_display : Widget {
		Window_text_display(Point XY, int W, int H, string L, Callback cb): Widget(XY, W, H, L, cb)
		{}
		void set_buffer(string s);
		void attach(Window&);
	private:
		Fl_Text_Buffer data;
		Fl_Text_Display *ptd;

	};
	void Window_text_display::attach(Window& win)
	{
		ptd = new Fl_Text_Display(loc.x, loc.y, width, height, label.c_str());
		ptd->buffer(&data);
		pw  = (Fl_Widget*) ptd;
		ptd->textfont(Fl_Font::FL_COURIER);
		pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
		own = &win;
	}
	void Window_text_display::set_buffer(string s) 
	{
		data.text(s.c_str());
	}

//------------------------------------------------------------------------------
	struct Menu_bar : Widget {
		Menu_bar(Point XY, int W, int H, Callback cb): Widget(XY, W, H, "", cb)
		{}
		void menu(Fl_Menu_Item *ma);
		void attach(Window&);
	private:
		Fl_Menu_Bar *pmb;
		Fl_Menu_Item *ma;

	};
	void Menu_bar::attach(Window& win)
	{
		pmb = new Fl_Menu_Bar(loc.x, loc.y, width, height);
		pmb->menu(ma);
		pw  = (Fl_Widget*) pmb;
		pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
		own = &win;
	}
	void Menu_bar::menu(Fl_Menu_Item *m) 
	{
		ma = m;
	}
}

//header declarations
void do_about_box(Window&);
// RMN add header declaration
void do_help_box(Window&);


void do_find_categ_sales(Window&, ROMS_Menu&); //EP C
void do_add_order_item(Window&, ROMS_Menu&); //EP C
void do_find_table_sales(Window&, ROMS_Menu&); // RMN C
void do_update_add_menu_item(Window&, ROMS_Menu&); // RMN C

void do_read(Window&, ROMS_Menu&, string, string, Msg_type);
void Main_Window_CB(Fl_Widget*, void*);
void Menu_Bar_CB (Fl_Widget*, void*);

//global data
bool main_window_click = false;
bool menu_bar_click = false;
bool main_click_enabled = true;//used to force serialization on clicks
int window_userdata;
int menu_bar_userdata; //EP C

//main window menu bar
//Note: for expediancy sake, to define the menu bar below we used an FLTK type Fl_Menu_Item. 
//This makes our program  directly tied to FLTK instead of Dr. Stroustrup's interface Graph_lib.
//We don't like this, but we did it to save time, which often happens in the real world.
//A better solution is to write code to hide this in the implementation of the Graph_lib::Menu_bar, but that means adding
//changes to it and getting general agreement with Dr. Stroustrup. Subsequent testing and distribution would take time we don't have.
//An extra credit exercise is to see if you can eliminate direct references in your code to FLTK types like we have here
Fl_Menu_Item menu_bar[] = {
 {"Read", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Recipes", 0,  (Fl_Callback*)Main_Window_CB, Address (Read_recipes_ingredients), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Orders", 0,  (Fl_Callback*)Main_Window_CB, Address (Read_orders), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Menu Items", 0,  (Fl_Callback*)Main_Window_CB, Address (Read_category_menu_items), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Show", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Orders", 0,  (Fl_Callback*)Main_Window_CB, Address (Show_order), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Recipes", 0,  (Fl_Callback*)Main_Window_CB, Address (Show_recipe), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Menu Items", 0,  (Fl_Callback*)Main_Window_CB, Address (Show_menu), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Categories", 0,  (Fl_Callback*)Main_Window_CB, Address (Show_categories), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Order Items", 0,  (Fl_Callback*)Main_Window_CB, Address (Show_order_items), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Find", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Category Sales", 0,  (Fl_Callback*)Main_Window_CB, Address (Find_category_sales), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Table Sales", 0,  (Fl_Callback*)Main_Window_CB, Address (Find_table_sales), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Menu Item Sales", 0,  (Fl_Callback*)Main_Window_CB, Address (Find_menu_item_sales), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Update", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Add Order Item", 0,  (Fl_Callback*)Main_Window_CB, Address (Update_add_order_item), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Add Menu Item", 0,  (Fl_Callback*)Main_Window_CB, Address (Update_add_menu_item), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Add Recipe", 0,  (Fl_Callback*)Main_Window_CB, Address (Update_add_recipe), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"About", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Info", 0,  (Fl_Callback*)Main_Window_CB, Address (About_info), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Help", 0,  (Fl_Callback*)Main_Window_CB, Address (About_help), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Exit", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Save and exit", 0,  (Fl_Callback*)Main_Window_CB, Address (Exit_program), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0}
};


//call back functions
void recps_cb(Address addr, Address) // callback for recipes button
{
	Main_Window_CB((Fl_Widget*) addr, Address (Tables_button));
}
void order_cb(Address addr, Address) // callback for orders button
{
	Main_Window_CB((Fl_Widget*) addr, Address (Orders_button));
}
void cat_cb(Address addr, Address) // callback for category button
{
	Main_Window_CB((Fl_Widget*) addr, Address (Categories_button));
}
void mi_cb(Address addr, Address) // callback for menu item button
{
	Main_Window_CB((Fl_Widget*) addr, Address (Menu_items_button));
}
void oi_cb(Address addr, Address) // callback for order item button
{
	Main_Window_CB((Fl_Widget*) addr, Address (Recipes_button)); // RMN B3 I think it is supposed to be order items though...
}
 void display_cb(Address addr, Address) // callback for display window
{
	Main_Window_CB((Fl_Widget*) addr, Address (Display_window));
}
 void menu_bar_cb(Address addr, Address)//callback for menu bar
 {
	 Menu_Bar_CB((Fl_Widget*) addr, Address (Menu_bar_cb));
 }

 //EP C
 template <Address T>
 void general_menu_bar_cb(Address addr, Address)//callback for menu bar
 {
	 Menu_Bar_CB((Fl_Widget*) addr, T);
 }

void Menu_Bar_CB (Fl_Widget*, void* ud) {
	cout << "Menu Bar Callback" << " Userdata=" << (int) ud << "\n";
	menu_bar_userdata = (int) ud; //EP C
	menu_bar_click = true;
}

void Main_Window_CB(Fl_Widget*, void* ud) {
	if(main_click_enabled)//don't signal the window click if main loop is active
	{
		cout << "Main Window Callback" << " Userdata=" << (int) ud << "\n";
		window_userdata = (int) ud;
		main_window_click = true;
		main_click_enabled = false; //main loop active on current click
	}
}
//wait functions for callbacks to be executed
void wait_for_main_window_click() {
	while (!main_window_click) {
		Fl::wait();
	}
	main_window_click = false;
	Fl::redraw();
}

void wait_for_menu_bar_click() {
	while (!menu_bar_click)
		Fl::wait();
	menu_bar_click = false;
	Fl::redraw();
}

int main()
{
	//create the main window and display it
	ROMS_Menu m;
    Window sw(600, 500, "Restaurant Order Management System");
    Button recps(Point(40, 50), 100, 30, "Recipes", recps_cb);
	sw.attach(recps);
	Button order(Point(40, 100), 100, 30, "Orders", order_cb);
	sw.attach(order);
	Button cat(Point(40, 150), 100, 30, "Categories", cat_cb);
	sw.attach(cat);
	Button mi(Point(40, 200), 100, 30, "Menu Items", mi_cb);
	sw.attach(mi);
	Button ci(Point(40, 250), 100, 30, "Order Items", oi_cb);
	sw.attach(ci);
    sw.callback((Fl_Callback*)Main_Window_CB, Address (Exit_program));
	Menu_bar mb(Point(0, 0), 600, 30, menu_bar_cb);
    mb.menu(menu_bar);
	sw.attach(mb);

	Window_text_display t(Point(150,50),400, 400, "Data Display", display_cb );
	t.set_buffer("No Data Loaded\n");
	sw.attach(t);
 	sw.resizable(&sw);
	

	//main loop to execute user clicks on the main window
	//do you understand how this loop works compared to the way the main loop worked in Part I of the project?
	try {
		
		while(true) {
			main_click_enabled = true;
			wait_for_main_window_click();
			cout << "Act on userdata = " << window_userdata << endl;
			switch(window_userdata) 
			{
				//need to complete all the menu cases
				case Read_recipes_ingredients:
					do_read(sw, m, "recipes.dat", "Open Recipes/Ingredients File", Read_recipes_ingredients);
					break;
				case Read_orders:
					do_read(sw, m, "orders.dat", "Open Order/Order Items File", Read_orders);
					break;
				case Read_category_menu_items:
					do_read(sw, m, "catmenu.dat", "Open Category/Menu Item File", Read_category_menu_items);
					break;
				case About_info:
					do_about_box(sw);
					break;
					// RMN add new case
				case About_help:
					do_help_box(sw);
					break;
				case Tables_button: case Orders_button: case Categories_button: case Recipes_button: case Menu_items_button:
				case Show_categories: case Show_order_items: // RMN B.3
				case Show_menu: case Show_recipe: case Show_order: //## Ecky B2
					t.set_buffer(m.show_button((Msg_type) window_userdata).c_str());
					break;
				case Exit_program:
					cout << "save files and exit\n";
					m.write_all(); //RCD B.1
					exit(0); //exit program
					break;
				//EP C
				case Find_category_sales:
					do_find_categ_sales(sw, m);
					break;
				//EP C
				case Update_add_order_item:
					do_add_order_item(sw, m);
					break;
				//RMN C
				case Find_table_sales:
					do_find_table_sales(sw,m);
					break;
				case Update_add_menu_item:
					do_update_add_menu_item(sw,m);
					break;
				default:
					cout << "case not implemented\n";
			}
		}
		return 0;
	}
	catch(exception& e) {
		cerr << "exception: " << e.what() << '\n';
		return 1;
	}
	catch (...) {
		cerr << "Some exception\n";
		return 2;
	}
}
//show about box info window
void do_about_box(Window& w) 
{
        Window ab(Point(w.x()+50, w.y()+50), 500, 400, "About Box"); //modified: RCD B.1
	ab.color(Color::white);
	ab.callback((Fl_Callback*)Menu_Bar_CB, Address (Close_about_box));
	//RCD B.1 ---
	Text msg(Point(40,25), "Ward Project Part II: Restaurant Order Management System");
	msg.set_color(Color::black);
	Text team(Point(215,50), "Team 4");
	team.set_color(Color::black);
	team.set_font(Font::helvetica_bold);
	Image ecky_image(Point(40, 60), "Ecky.gif");
	Text ecky(Point(146, 80), "Ecky Putrady");
	ecky.set_color(Color::black);
	Image rizky_image(Point(40, 166), "Rizky.gif");
	Text rizky(Point(146, 186), "Rizky Nugraha");
	rizky.set_color(Color::black);
	Image ross_image(Point(40, 272), "Ross.gif");
	Text ross(Point(146, 292), "Ross Dixon");
	ross.set_color(Color::black);
	ab.attach(msg);
	ab.attach(team);
	ab.attach(ecky_image);
	ab.attach(ecky);
	ab.attach(rizky_image);
	ab.attach(rizky);
	ab.attach(ross_image);
	ab.attach(ross);
	//end RCD B.1 ---
	wait_for_menu_bar_click();
	ab.detach(msg);//clean up window
	return;
}

// RMN create help window
void do_help_box(Window& w)
{
	Window ab(Point(w.x()+100, w.y()+100), 500, 220, "Help Box");
	ab.color(Color::white);
	ab.callback((Fl_Callback*)Menu_Bar_CB, Address (Close_about_box));
	Text msg[]={
		Text(Point(15,50), "Restaurant Order Management System\n"),
		Text(Point(15,70), "\n"),
		Text(Point(15,90), "Use Read menu to read the data from file\n"),
		Text(Point(15,110), "Use Show menu to show the data\n"),
		Text(Point(15,130), "Use Find menu to find out about the restaurant's sales\n"),
		Text(Point(15,150), "Use Update menu to change data\n"),
		Text(Point(15,170), "Use About menu to get information about this program and get help\n"),
		Text(Point(15,190), "Use Exit menu to exit and write changes to files\n")
	};
	for(int i=0;i<7;i++)
	{
		msg[i].set_color(Color::black);
		ab.attach(msg[i]);
	}
	wait_for_menu_bar_click();
	for(int i=0;i<7;i++)
	{
		ab.detach(msg[i]);//clean up window
	}
	return;
}

void do_read(Window& w, ROMS_Menu& m, string dfn, string msg, Msg_type type)
{
	//Here again we linked our code directly to the FLTK library instead of creating an interface in our Graph_lib interface to FLTK
	//We did it for lack of time, which reflects the real word. Unfortunately if we wanted to move our code to a different graphics
	//library, we have to change our code and not just our Graphic_lib implementation. we are using the FLTK file chooser directly.
	Fl_File_Chooser fc(dfn.c_str(), "*.dat", Fl_File_Chooser::CREATE, msg.c_str());
	fc.show();
	while (fc.shown())
		Fl::wait();	//wait for OK or Cancel button
	if(0!=fc.value())//make sure there is a string to use as a filename
	{
		string fname = fc.value();
		switch(type) {
			case Read_recipes_ingredients:
				m.read_recipes_ingredients(fname);
				break;
			case Read_orders:
				m.Read_orders(fname);
				break;
			case Read_category_menu_items:
				m.read_catprds(fname);
				break;
			default:
				throw InvalidType();
		}
	}
	return;
}

//EP C
void do_find_categ_sales(Window& w, ROMS_Menu& menu) {
	//create window
	Window ab(Point(w.x()+100, w.y()+100), 300, 80, "Add Order Item");
	ab.color(Color::white);
	ab.callback((Fl_Callback*)Menu_Bar_CB, Address (Close_about_box));

	////create UI Components
	//text
	Text sel_cat_txt(Point(10, 25), "Select Category:");
	ab.attach(sel_cat_txt);

	//drop down
	Fl_Choice c(120, 10, 100, 20);
	ab.add(c);
	c.callback((Fl_Callback*)Menu_Bar_CB, Address(Find_category_sales));
	vector<Category>& categories = menu.get_categories();
	for(int i = 0; i < categories.size(); ++i) {
		c.add(categories[i].name().c_str());
	}

	//text
	Text sum_txt(Point(120, 50), "");
	ab.attach(sum_txt);

	bool exit = false;
	while(!exit) {
		wait_for_menu_bar_click();
		//if drop down menu changed
		if(menu_bar_userdata == Find_category_sales) {
			stringstream ss;
			ss << "Total Sales: $" << menu.get_category_total_sales(categories[c.value()].get_cat_id());
			sum_txt.set_label(ss.str());
		}
		
		//anything else
		else {
			exit = true;
		}
	}
}

//EP C
void do_add_order_item(Window& w, ROMS_Menu& menu) {
	//create window
	Window ab(Point(w.x()+100, w.y()+100), 220, 220, "Add Order Item");
	ab.color(Color::white);
	ab.callback((Fl_Callback*)Menu_Bar_CB, Address (Close_about_box));

	////create UI components
	//params
	int input_height  = 20;
	int input_offset_x= 100;
	int input_offset_y= 10;
	int input_spacing = 30;
	int input_width = 100;

	//texts
	Text seat_id_txt		(Point(5, input_offset_y + input_spacing * 0 + 15), "Seat ID");
	Text order_id_txt		(Point(5, input_offset_y + input_spacing * 1 + 15), "Order ID");
	Text menu_item_id_txt	(Point(5, input_offset_y + input_spacing * 2 + 15), "Menu Item ID");
	Text prod_qty_txt		(Point(5, input_offset_y + input_spacing * 3 + 15), "Qty");
	Text status_txt			(Point(5, input_offset_y + input_spacing * 4 + 15), "");
	ab.attach(seat_id_txt);
	ab.attach(order_id_txt);
	ab.attach(menu_item_id_txt);
	ab.attach(prod_qty_txt);
	ab.attach(status_txt);
	
	//inputs
	Fl_Input seat_id		(input_offset_x, input_offset_y + input_spacing * 0, input_width, input_height);
	Fl_Input order_id		(input_offset_x, input_offset_y + input_spacing * 1, input_width, input_height);
	Fl_Input menu_item_id	(input_offset_x, input_offset_y + input_spacing * 2, input_width, input_height);
	Fl_Input prod_qty		(input_offset_x, input_offset_y + input_spacing * 3, input_width, input_height);
	ab.add(seat_id);
	ab.add(order_id);
	ab.add(menu_item_id);
	ab.add(prod_qty);

	//button
	Button add(
		Point(input_offset_x, input_offset_y + input_spacing * 5), 
		input_width, 30, 
		"Add", 
		general_menu_bar_cb<Address(Update_add_order_item)>
	);
	ab.attach(add);
	
	//process this window's logic
	bool exit = false;
	while(!exit) {
		//wait for anything to happen at the window
		wait_for_menu_bar_click();

		////parse command
		//user click the add button
		if(menu_bar_userdata == Update_add_order_item) {
			//create an order_item based on user's input
			stringstream ss;
			ss << seat_id.value() << " " << order_id.value() << " " << menu_item_id.value() << " " << prod_qty.value();
			Order_Item oi;
			if(!(ss >> oi)) {
				status_txt.set_label("Invalid Input!");
			} else {
				//add that item to database
				string msg;
				exit = menu.addOrderItem(oi, msg);
				status_txt.set_label(msg);
			}

		}
		
		//user click other things
		else {
			exit = true;
		}
	}
}

// RMN C create table sales window
void do_find_table_sales(Window& w, ROMS_Menu& menu) 
{
	//create window
	Window ab(Point(w.x()+100, w.y()+100), 300, 80, "Table Sales");
	ab.color(Color::white);
	ab.callback((Fl_Callback*)Menu_Bar_CB, Address (Close_about_box)); // it is bad, but just reuse the Close_about_box

	////create UI Components
	//text
	Text sel_table_txt(Point(10, 25), "Select Table ID:");
	ab.attach(sel_table_txt);

	//drop down
	Fl_Choice c(120, 10, 100, 20);
	ab.add(c);
	c.callback((Fl_Callback*)Menu_Bar_CB, Address(Find_table_sales)); // again, reuse
	vector<int> table_ids(menu.get_table_ids());
	for(unsigned int i=0;i<table_ids.size();i++)
	{
		stringstream s;
		s<<table_ids[i];
		c.add(s.str().c_str()); // too many str... lol...
	}

	//text
	Text sum_txt(Point(120, 50), "");
	ab.attach(sum_txt);

	bool exit = false;
	while(!exit) {
		wait_for_menu_bar_click();
		//if drop down menu changed
		if(menu_bar_userdata == Find_table_sales) {
			stringstream ss;
			stringstream value;
			value<<c.value();
			int id;
			value>>id;
			ss << "Total Sales: $" << menu.get_table_sales(table_ids[id]);
			sum_txt.set_label(ss.str());
		}
		
		//anything else
		else {
			exit = true;
		}
	}
}

// RMN C
void do_update_add_menu_item(Window& w, ROMS_Menu& m)
{
	int id,cat_id,rec_id;
	string name,descr_text;
	double amt;

	// Create new window
	Window ab(Point(w.x()+100, w.y()+100), 320, 320, "Add Menu Item");
	ab.color(Color::white);
	ab.callback((Fl_Callback*)Menu_Bar_CB, Address (Close_about_box));

	////create UI components
	//params
	int input_height  = 20;
	int input_offset_x= 100;
	int input_offset_y= 10;
	int input_spacing = 30;
	int input_width = 100;

	//texts
	Text menu_item_id_txt  (Point(5, input_offset_y + input_spacing * 0 + 15), "Menu Item ID");
	Text category_id_txt		(Point(5, input_offset_y + input_spacing * 1 + 15), "Category ID");
	Text recipe_id_txt	(Point(5, input_offset_y + input_spacing * 2 + 15), "Recipe ID");
	Text menu_item_name_txt	(Point(5, input_offset_y + input_spacing * 3 + 15), "Menu Name");
	Text price_txt		(Point(5, input_offset_y + input_spacing * 4 + 15), "Price");
	Text description_txt			(Point(5, input_offset_y + input_spacing * 5 + 15), "Description");
	Text status_txt		(Point(5, input_offset_y + input_spacing * 6 + 15), "");
	ab.attach(menu_item_id_txt);
	ab.attach(category_id_txt);
	ab.attach(recipe_id_txt);
	ab.attach(menu_item_name_txt);
	ab.attach(price_txt);
	ab.attach(description_txt);
	ab.attach(status_txt);

	//inputs
	Fl_Input menu_item_id	(input_offset_x, input_offset_y + input_spacing * 0, input_width, input_height);
	Fl_Choice category_id	(input_offset_x, input_offset_y + input_spacing * 1, input_width, input_height);
	Fl_Choice recipe_id		(input_offset_x, input_offset_y + input_spacing * 2, input_width, input_height);
	Fl_Input menu_item_name	(input_offset_x, input_offset_y + input_spacing * 3, input_width, input_height);
	Fl_Input price			(input_offset_x, input_offset_y + input_spacing * 4, input_width, input_height);
	Fl_Input description	(input_offset_x, input_offset_y + input_spacing * 5, input_width, input_height);

	vector<Category> categories=m.get_categories();
	vector<Recipe> recipes=m.get_recipes();
	for(unsigned int i=0;i<categories.size();i++)
	{
		stringstream ss;
		ss<<categories[i].name();
		category_id.add(ss.str().c_str());
	}

	for(unsigned int i=0;i<recipes.size();i++)
	{
		stringstream ss;
		ss<<recipes[i].get_rec_id(); // we can only display recipe_id
		recipe_id.add(ss.str().c_str());
	}

	ab.add(menu_item_id);
	ab.add(category_id);
	ab.add(recipe_id);
	ab.add(menu_item_name);
	ab.add(price);
	ab.add(description);

	//button
	Button add(
		Point(input_offset_x, input_offset_y + input_spacing * 7), 
		input_width, 30, 
		"Add", 
		general_menu_bar_cb<Address(Update_add_menu_item)>
	);
	ab.attach(add);
	
	//process this window's logic
	bool exit = false;
	while(!exit) {
		//wait for anything to happen at the window
		wait_for_menu_bar_click();

		////parse command
		//user click the add button
		if(menu_bar_userdata == Update_add_menu_item) {
			//create an order_item based on user's input
			// assumed that all input is valid;
			stringstream ss;
			ss<<menu_item_id.value()<<" "<<category_id.value()<<" "<<recipe_id.value()<<" "<<price.value();
			ss>>id;
			int c_id;
			int r_id;
			ss>>c_id;
			ss>>r_id;
			ss>>amt;
			string msg;
			name=menu_item_name.value();
			descr_text=description.value();
			if(!ss || ss.fail() || name.size()==0 || descr_text.size()==0)
			{
				msg="Invalid Input";
			}
			else
			{
				cat_id=categories[c_id].get_cat_id();
				rec_id=recipes[r_id].get_rec_id();
				Description desc(descr_text);
				Menu_Item menu_item(id,cat_id,rec_id,name,amt,desc);
				exit=m.addMenuItem(menu_item,msg);
			}
			status_txt.set_label(msg);
		}
		
		//user click other things
		else {
			exit = true;
		}
	}

}