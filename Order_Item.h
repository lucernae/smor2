#ifndef ORDER_ITEM_H_
#define ORDER_ITEM_H_

#include <string>
#include <sstream>
#include <iomanip> // RMN add include
using namespace std;


struct Order_Item {
public:
	// constructors
	Order_Item () {}
	Order_Item(string sid, int oid, int miid, int n):
	seat_id(sid), order_id(oid), menu_item_id(miid), prod_qty(n) {}
   // utility functions
	static string display_headers() {
		ostringstream oss;
		oss << left << setw(9) << "Seat ID" << left << setw(10) << "Order ID" << left << setw(13) << "Menu Item ID" << left << setw(9) << "Quantity";
		return oss.str();
	}
	string display() const {	
		ostringstream oss;
		//oss << seat_id << " " << order_id << " " << menu_item_id << " " << prod_qty ;
		oss << left << setw(9) << seat_id << left << setw(10) << order_id << left << setw(13) << menu_item_id << left << setw(9) << prod_qty;
		return oss.str();
	}
	int get_order_id() const {return order_id;}	//B.2.a
	int get_qty() const {return prod_qty;}	//B.2.a
	int get_menu_item_id() const {return menu_item_id;}	//B.2.a
private:
   //constants  

   // private data
	string seat_id;
	int order_id;
	int menu_item_id;
	int prod_qty;
};


#endif //ORDER_ITEM_H_