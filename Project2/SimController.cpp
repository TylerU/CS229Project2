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

int WireWorldSimRunner::getNumNeighborHeads(Grid *grid, int x, int y){
	int sum = 0;
	for(int dx = -1; dx <= 1; dx++){
		for(int dy = -1; dy <= 1; dy++){
			if(dx == 0 && dy == 0){
			}
			else{
				if(grid->getStateOfCoord(x+dx, y+dy) == "Head"){
					sum++;
				}
			}
		}
	}
	return sum;
}


void WireWorldSimRunner::runOnPoint(Grid* grid, int x, int y){
	string state = grid->getStateOfCoord(x, y);
	if(state == "Empty"){
		grid->setStateOfCoord(x, y, "Empty");
	}
	else if (state == "Head"){
		grid->setStateOfCoord(x, y, "Tail");
	}
	else if (state == "Tail"){
		grid->setStateOfCoord(x, y, "Wire");
	}
	else if (state == "Wire"){
		int numHeads = getNumNeighborHeads(grid, x, y);
		if(numHeads == 1 || numHeads == 2){
			grid->setStateOfCoord(x, y, "Head");
		}
		else{
			grid->setStateOfCoord(x, y, "Wire");
		}
	}
}

int ElementarySimRunner::getPattern(Grid *grid, int x, int y){
	int pat = 0;
	if(grid->getStateOfCoord(x-1, y+1) == "One"){
		pat += 4;
	}

	if(grid->getStateOfCoord(x, y+1) == "One"){
		pat += 2;
	}

	if(grid->getStateOfCoord(x+1, y+1) == "One"){
		pat += 1;
	}

	return pat;
}


int ElementarySimRunner::getNthBitOf(int n, int num){
	return (num & (1 << n)) >> n;
}


void ElementarySimRunner::runOnPoint(Grid* grid, int x, int y){
	string state = grid->getStateOfCoord(x, y);
	if(state == "One"){
		grid->setStateOfCoord(x, y, "One");
	}
	else{//Assume "Zero"
		int pattern = getPattern(grid, x, y);
		int bit = getNthBitOf(pattern, rule);
		if(bit == 0){
			grid->setStateOfCoord(x, y, "Zero");
		}
		else{
			grid->setStateOfCoord(x, y, "One");
		}
	}
}


const PairList * SimController::getPairListForState(string strstate){
	return grid->getPairListForState(strstate);
}

void SimController::simGeneration(){
	gen++;
	grid->startChangeBuffering();
	mySim->run(grid);
	grid->applyChangeBuffer();
}

SimController::SimController(SimOptions *options, SimRunner *sim){
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