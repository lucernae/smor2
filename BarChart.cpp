#include "BarChart.h"
#include "Graph.h"

using namespace Graph_lib;

BarChart::~BarChart(void)
{
	removeAllShape();
}

// RMN some nasty and complicated init function, but i implement it as flexible as possible...
//	time spent here : 4 hours
//	and it takes my sleep time... lol... 3 hours spent wondering what to do with the shapes objects...
//	oh man... and another hour to style the chart... and a couple of minute typing comment because i had
//	nothing to do...
void BarChart::initChartValue(string legendH, string legendV, vector<string>* labels, vector<double>* src, Color color) {
	//assign vars
	_legend_h	= legendH;
	_legend_v	= legendV;
	_labels		= labels;
	_src		= src;


	// RMN initialize axis label
	initAxisLabel(legendH,legendV,labels,src);

	// RMN now we draw the value...
	Shape * s;
	int barLen=0;
	int padding=5;
	for(int i=0;i+page<src->size() && i<maxXLabel;i++)
	{
		if(maxValue!=0)
		{
			barLen=(int)(src->at(i+page)*height/maxValue);
			//cout << "debug barlen " << i << "\n";
		}
		else
		{
			barLen=0;
		}
		if(barLen>0 && notchW>0)
		{
			//cout << "debug notch " << i << "\n";
			s=new Rectangle(Point(loc.x+i*notchW+padding,loc.y+height-barLen),notchW-2*padding,barLen);
			s->set_fill_color(color);
			s->set_color(color);
			addShape(s);
		}
	}
}

void BarChart::initPercentageChartValue(string legendH, string legendV,vector<string>* legendCategory,vector<string> * labels, vector<vector<double>>* src,vector<Color>* color)
{
	// RMN initialize axis label
	vector<double> dsrc;
	dsrc.push_back(100);
	initAxisLabel(legendH,legendV,labels,&dsrc);

	// percentage the value...
	vector<vector<double>> srcPercentage;

	for(int i=0;i+page<src->size() && i<maxXLabel;i++)
	{
		double total=0;
		int index=i+page;
		for(int j=0;j<src->at(index).size();j++)
		{
			total+=src->at(index).at(j);
		}
		vector<double> percent;
		// check to avoid division by zero
		if(total!=0)
		{
			for(int j=0;j<src->at(index).size();j++)
			{
				percent.push_back(src->at(index).at(j)*100/total);
			}
		}
		srcPercentage.push_back(percent);
	}


	Shape * s;
	// RMN now we draw the value...
	int barLen=0;
	int padding=5;
	for(int i=0;i+page<srcPercentage.size() && i<maxXLabel;i++)
	{
		int totalBar=0;
		int index=i+page;
		for(int j=0;j<srcPercentage.at(index).size();j++)
		{
			barLen=(int)(srcPercentage.at(index).at(j)*height/100);
			if((int)barLen>0)
			{
				s=new Rectangle(Point(loc.x+i*notchW+padding,loc.y+height-totalBar-barLen),notchW-2*padding,barLen);
				s->set_fill_color(color->at(j));
				s->set_color(color->at(j));
				addShape(s);
				totalBar+=barLen;
			}
		}
	}
	// RMN add legend
	int legendspace=20;
	int legendmargin=70;

	for(int i=0;i<color->size();i++)
	{
		s=new Rectangle(Point(loc.x,loc.y+height+legendmargin+legendspace*(i-1)),legendspace,legendspace);
		s->set_fill_color(color->at(i));
		addShape(s);
		s=new Text(Point(loc.x+legendspace*2,loc.y+height+legendmargin+legendspace*i),legendCategory->at(i));
		addShape(s);
	}
}

void BarChart::addShape(Shape* s)
{
	shapes.push_back(s);
	own->attach(*s);
}

void BarChart::removeAllShape()
{
	// RMN yeah... just delete everything....
	if(shapes.size()>0)
	{
		for(int i=0;i<shapes.size();i++)
		{
			Shape* s=shapes[i];
			own->detach(*s);
			delete s;
		}
		shapes.clear();
	}
}

void BarChart::initAxisLabel(string legendH, string legendV, vector<string>* labels, vector<double>* src)
{
	//destroy previous attached shapes (if any)
	removeAllShape();

	//attach new shapes to the window ... (window an be referenced to own)

	// RMN create a new object along the way and store its reference.
	//	You know, I cannot instantiate any local variable to use, because if I do that,
	//	the engine cannot draw the shapes. The engine can't draw the shapes because the
	//	object is gone... well, because it is local variable...
	//	So, instead making local variable like: Axis xAxis(...,...,...) , we had to use 'new'
	//	keywords and carefully deleted it, when it is not required anymore
	Shape *s;
	
	adjustPage(labels->size());
	
	int minimalNotchW=12;
	if(width/maxXLabel<minimalNotchW)
	{
		notchW=minimalNotchW;
	}
	else
	{
		notchW=width/maxXLabel;
	}

	// RMN x axis. The Axis is placed so it will become a widget box when it is drawn
	s=new Axis(Axis::Orientation::x,Point(loc.x,loc.y+height),notchW*maxXLabel,maxXLabel,"");
	s->set_color(Color::red);
	addShape(s);

	// RMN y axis. same here...
	int yNotch=10; // Y not? Just kidding...
	s=new Axis(Axis::Orientation::y,Point(loc.x,loc.y+height),height,yNotch,"");
	s->set_color(Color::red);
	addShape(s);

	// RMN horizontal labels. We carefully take the size of the notch to match this size of labels
	for(int i=0;i+page<labels->size() && i<maxXLabel;i++)
	{
		addShape(new Text(Point(loc.x+(i)*notchW,loc.y+height+15),labels->at(i+page)));
	}

	// RMN vertical labels. We also carefully take the size of the notch and its label to match src vector
	maxValue=0;
	for(int i=0;i<src->size();i++)
	{
		if(src->at(i)>maxValue)
		{
			maxValue=src->at(i);
		}
	}
	notchH=height/yNotch;
	notchHValue=maxValue/yNotch;
	if(notchHValue==0)
	{
		notchHValue=10; // RMN set default value, in case src contains nothing
	}
	for(int i=0;i<=yNotch;i++)
	{
		stringstream stream;
		stream<<notchHValue*i;
		addShape(new Text(Point(loc.x-10*stream.str().size(),loc.y+height-i*notchH+10),stream.str()));
	}

	// RMN draw the legendH and legendV
	addShape(new Text(Point(loc.x+width/2-legendH.size()*10/2,loc.y+height+50),legendH));
	addShape(new Text(Point(loc.x-legendV.size()*30/2,loc.y+height/2),legendV));
}

void BarChart::nextPage()
{
	page+=maxXLabel;
}

void BarChart::prevPage()
{
	page-=maxXLabel;
}

void BarChart::adjustPage(int xAxisSize)
{
	if(page>xAxisSize)
	{
		page=(page/xAxisSize)*xAxisSize;
	}
	else if(page==xAxisSize)
	{
		page-=maxXLabel;
	}
	else if(page<0)
	{
		page=0;
	}
}