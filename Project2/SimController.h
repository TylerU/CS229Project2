#ifndef SIMCONTROLLER_H
#define SIMCONTROLLER_H
#include "SimOptions.h"
#include "Grid.h"
#include <stdexcept>

using namespace std;


class SimRunner{
public:
	virtual void run(Grid* grid);
	virtual void runOnPoint(Grid *grid, int x, int y) =0;
};

class LifeSimRunner : public SimRunner{
private:
	int getNearAlive(Grid *grid, int x, int y);
public:
	virtual void runOnPoint(Grid *grid, int x, int y);
};

class SimController{
protected:
	int gen;
	const SimOptions *opts;
	Grid *grid;
	SimRunner *mySim;
protected:
	void simGeneration();
public:
	SimController(SimOptions *options, SimRunner *sim);
	virtual string getStateOfCoord(int x, int y);
	virtual void simGenerations(int g);
	virtual const PairList * getPairListForState(string state);
	inline int getCurGeneration() { return gen; }
	virtual ~SimController();
};
#endif