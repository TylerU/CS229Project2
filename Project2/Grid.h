#ifndef GRID_H
#define GRID_H
#include <string>
#include <map>
#include "SimOptions.h"
#include <stdexcept>

using namespace std;

class PointSetBuffer{
public:
	PointSetBuffer(Pair c, string s) : coord(c.getFirst(), c.getSecond()), id(s) {}
	Pair coord;
	string id;
};

class Grid{
protected:
	map<string, PairList *> *state;
	string defaultState;
	Range xRange;
	Range yRange;
	vector<PointSetBuffer> curChangeBuffer;
	bool buffering;
protected:
	void removeFromAllStates(Pair p);
	void addToState(string state, Pair p);
	void addStartState(string id, PairList *list);
	void addToState(Pair p, string s);
public:
	Grid(SimOptions *opts);
	virtual string getStateOfCoord(int x, int y);
	virtual PairList * getPairListForState(string state);
	virtual void setStateOfCoord(int x, int y, string s);
	void setXRange(Range r);
	void setYRange(Range r);
	Range getXRange();
	Range getYRange();
	void startChangeBuffering();
	void applyChangeBuffer();
	virtual ~Grid();
};
#endif