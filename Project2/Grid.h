#include <string>
#include <map>
#include "SimOptions.h"

using namespace std;

class Grid{
protected:
	map<string, PairList *> *state;
	string defaultState;
	Range xRange;
	Range yRange;
protected:
	void removeFromAllStates(Pair p);
	void addToState(string state, Pair p);
	void addStartState(string id, PairList *list);
	void addToState(Pair p, string s);
public:
	Grid(Range xRange, Range yRange, SimOptions *opts);
	virtual string getStateOfCoord(int x, int y);
	virtual const PairList const* getPairListForState(string state);
	virtual void setStateOfCoord(int x, int y, string s);
	void setXRange(Range r);
	void setYRange(Range r);
	Range getXRange();
	Range getYRange();
	virtual ~Grid();
};