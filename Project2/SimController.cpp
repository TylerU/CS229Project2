#include "SimController.h"

void SimRunner::run(Grid *grid){
	Range xRange = grid->getXRange();
	Range yRange = grid->getYRange();
	for(int y = yRange.getLow(); y <= yRange.getHigh(); y++){
		for(int x = xRange.getLow(); x<= xRange.getHigh(); x++){
			runOnPoint(grid, x, y);
		}
	}
}


int LifeSimRunner::getNearAlive(Grid *grid, int x, int y){
	int sum = 0;
	for(int dx = -1; dx <= 1; dx++){
		for(int dy = -1; dy <= 1; dy++){
			if(dx == 0 && dy == 0){
			}
			else{
				if(grid->getStateOfCoord(x+dx, y+dy) == "Alive"){
					sum++;
				}
			}
		}
	}
	return sum;
}

void LifeSimRunner::runOnPoint(Grid* grid, int x, int y){
	int nearbyAlive = getNearAlive(grid, x, y);
	string state = grid->getStateOfCoord(x, y);
	if(state == "Alive"){
		if(nearbyAlive < 2){
			grid->setStateOfCoord(x, y, "Dead");
		}
		else if(nearbyAlive == 2 || nearbyAlive == 3){
			grid->setStateOfCoord(x, y, "Alive");
		}
		else{
			grid->setStateOfCoord(x, y, "Dead");
		}
	}
	else{//Assume dead
		if(nearbyAlive == 3){
			grid->setStateOfCoord(x, y, "Alive");
		}
	}
}

const PairList const* SimController::getPairListForState(string strstate){
	return grid->getPairListForState(strstate);
}

void SimController::simGeneration(){
	gen++;
	grid->startChangeBuffering();
	mySim->run(grid);
	grid->applyChangeBuffer();
}

SimController::SimController(const SimOptions *options, SimRunner *sim){
	mySim = sim;
	gen = 0;
	opts = options;
	grid = new Grid(options);
}

string SimController::getStateOfCoord(int x, int y){
	return grid->getStateOfCoord(x,y);
}

void SimController::simGenerations(int g){
	for(int i = 0; i < g; i++){
		simGeneration();
	}
}

SimController::~SimController(){
	delete grid;
}