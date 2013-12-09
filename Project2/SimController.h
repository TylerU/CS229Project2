#ifndef SIMCONTROLLER_H
#define SIMCONTROLLER_H
#include "SimOptions.h"
#include "Grid.h"
#include <stdexcept>

using namespace std;

/*
	Runs a generation of any automata on a given grid. 
*/
class SimRunner{
public:
	virtual void run(Grid* grid);
	virtual void runOnPoint(Grid *grid, int x, int y) =0;
};

/*
	Conway's Game of Life's SimRunner. 
*/
class LifeSimRunner : public SimRunner{
private:
	int getNearAlive(Grid *grid, int x, int y);
public:
	virtual void runOnPoint(Grid *grid, int x, int y);
};

/*
	Wolfram's Elementary Automata SimRunner. 
*/
class ElementarySimRunner : public SimRunner{
private:
	int rule;
	/*Returns an int representation of the binary pattern (the top 3 neighbor tiles)*/
	int getPattern(Grid *grid, int x, int y);
	int getNthBitOf(int n, int num);
public:
	ElementarySimRunner(int r) {rule = r;}
	virtual void runOnPoint(Grid *grid, int x, int y);
};

/*
	Wire world sim runner.
*/
class WireWorldSimRunner : public SimRunner{
private:
	/* Returns the number of neighbors that are heads */
	int getNumNeighborHeads(Grid *grid, int x, int y);
public:
	virtual void runOnPoint(Grid *grid, int x, int y);
};

/*
	A controller for a simulation. given an options object and a SimRunner, allows modification of the game state
*/
class SimController{
protected:
	int gen;
	SimOptions *opts;
	Grid *grid;
	SimRunner *mySim;
protected:
	void simGeneration();
public:
	SimController(SimOptions *options, SimRunner *sim);
	virtual string getStateOfCoord(int x, int y);
	virtual void simGenerations(int g);
	/* Returns a PairList of all the blocks/tiles that are in a given state */
	virtual const PairList *getPairListForState(string state);
	inline int getCurGeneration() { return gen; }
	virtual ~SimController();
	virtual void reset();
};
#endif
