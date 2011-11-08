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

void do_read(Window&, ROMS_Menu&, string, string, Msg_type);
void Main_Window_CB(Fl_Widget*, void*);
void Menu_Bar_CB (Fl_Widget*, void*);

//global data
bool main_window_click = false;
bool menu_bar_click = false;
bool main_click_enabled = true;//used to force serialization on clicks
int window_userdata;

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

void Menu_Bar_CB (Fl_Widget*, void* ud) {
	cout << "Menu Bar Callback" << " Userdata=" << (int) ud << "\n";
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
		Text(Point(15,50), "Restaurant Order Management System\n\n"),
		Text(Point(15,70), ""),
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
