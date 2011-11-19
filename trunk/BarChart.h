#ifndef _BAR_CHART_
#define _BAR_CHART_

#include "Simple_window.h"
#include "std_lib_facilities.h"

using namespace std;

class BarChart : public Widget
{
public:
	BarChart(Point XY, int W, int H, string L, Callback cb): Widget(XY, W, H, L, cb) {}
	~BarChart(void);

	void init(string legendH, string legendV, vector<string>* labels, vector<double>* src);
	virtual void attach(Window& win){ own = &win; };

private:
	vector<string>* _labels;
	vector<double>* _src;
	string			_legend_h;	//horizontal legend
	string			_legend_v;	//vertical legend
};

#endif