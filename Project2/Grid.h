#ifndef GRID_H
#define GRID_H
#include <string>
#include <map>
#include "SimOptions.h"
#include <stdexcept>

using namespace std;

/*
	Contains a command, such as 'Set point (1,1) to state "Alive"', which can be buffered, then applied at a later time.
*/
class PointSetBuffer{
public:
	PointSetBuffer(Pair c, string s) : coord(c.getFirst(), c.getSecond()), id(s) {}
	Pair coord;
	string id;
};

/*
	Stores the grid and allows manipulation as well as buffered manipulation.
*/

class Grid{
protected:
	string **grid;
	string defaultState;
	Range xRange;
	Range yRange;
	vector<PointSetBuffer> curChangeBuffer;
	vector<string> validIds;
	bool buffering;
protected:
	void setAllToDefaultState();
	bool vectorContains(vector<string> vec, string str);
	bool withinTerrain(int x, int y);
	int translateTerrainXToGridX(int);
	int translateTerrainYToGridY(int);
	void addStartState(string id, PairList *list);
public:
	Grid(SimOptions *opts);
	virtual string getStateOfCoord(int x, int y);
	virtual PairList * getPairListForState(string state);
	virtual void setStateOfCoord(int x, int y, string s);
	void setXRange(Range r);
	void setYRange(Range r);
	Range getXRange();
	Range getYRange();
	/*
		Sets internal variables such that setStateOfCoord commands until applyChangeBuffer() call are stored rather than applied right away.
	*/
	void startChangeBuffering();
	/*
		Stops buffering and applys all changes in the buffer
	*/
	void applyChangeBuffer();
	virtual ~Grid();
};
#endif
