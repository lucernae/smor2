/*
RMN Ecky made the interface, and I implement it...
*/

#ifndef _BAR_CHART_
#define _BAR_CHART_

#include "Simple_window.h"
#include "std_lib_facilities.h"
#include "Graph.h"
#include "GUI.h"

using namespace std;

class BarChart : public Widget
{
public:
	BarChart(Point XY, int W, int H, string L, Callback cb): Widget(XY, W, H, L, cb) {
	}
	~BarChart(void);

	// RMN let's just decompose the graphing task into smaller task
	void addShape(Shape*s); // RMN function to add a shape so the shape can be carefully deleted later
	void removeAllShape(); // RMN to delete and detach all shape previously drawn
	void initAxisLabel(string legendH, string legendV,vector<string>* labels, vector<double>* src); // RMN this is to draw axis label
	void initChartValue(string legendH, string legendV, vector<string>* labels, vector<double>* src, Color color); // RMN this is to draw the value as vertical bar. Guess what? you can choose you own color... Am I such a good guy?
	void initPercentageChartValue(string legendH, string legendV,vector<string> * legendCategory,vector<string>* labels, vector<vector<double>>* src,vector<Color>* color); // RMN this is to draw percentage value as in extra credit 3
	//void init(string legendH, string legendV, vector<string>* labels, vector<double>* src);
	void attach(Window& win) { own=&win; }

private:
	vector<string>* _labels;
	vector<double>* _src;
	string			_legend_h;	//horizontal legend
	string			_legend_v;	//vertical legend
	vector<Shape*> shapes; // RMN need a vector of pointer to hold up reference when we need to detach shapes we drew
	int notchW; // RMN pixel width of horizontal notch
	int notchH; // RMN pixel height of vertical notch
	int notchHValue; // RMN notch value in vertical unit
	int maxValue; // RMN maximum value of data
};

#endif