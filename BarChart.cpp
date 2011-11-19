#include "BarChart.h"
#include "Graph.h"

using namespace Graph_lib;

BarChart::~BarChart(void)
{
}


void BarChart::init(string legendH, string legendV, vector<string>* labels, vector<double>* src) {
	//assign vars
	_legend_h	= legendH;
	_legend_v	= legendV;
	_labels		= labels;
	_src		= src;

	//destroy previous attached shapes (if any)

	//attach new shapes to the window ... (window an be referenced to own)
}