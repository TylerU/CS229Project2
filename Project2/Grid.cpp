#include "Grid.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <stdio.h>

#define ABS(X) ((X) < 0) ? -(X) : (X)

using namespace std;

bool Grid::vectorContains(vector<std::string> vec, std::string str){
	return std::find(vec.begin(), vec.end(), str) != vec.end();
}

Grid::Grid(SimOptions *opts){
	xRange = opts->terrainX;
	yRange = opts->terrainY;
	int sizex = (ABS(xRange.getHigh() - xRange.getLow())+1);
	int sizey = (ABS(yRange.getHigh() - yRange.getLow())+1);
	grid = new string*[sizex];
	for(int i = 0; i < sizex; i++) grid[i] = new string[sizey];

	buffering = false;
	defaultState = opts->getDefaultStateString();
	validIds = opts->getValidIdentifiers();
	
	setAllToDefaultState();
	for(int i = 0; i < validIds.size(); i++){
		if(validIds[i] != defaultState){
			addStartState(validIds[i], opts->getInitialList(validIds[i]));
		}
	}
}

void Grid::addStartState(string id, PairList *list){
	const vector<Pair> *pairList = list->getPairVector();
	for(int i = 0; i < pairList->size(); i++){
		Pair his = pairList->at(i);
		setStateOfCoord(his.getFirst(), his.getSecond(), id);
	}
}


string Grid::getStateOfCoord(int x, int y){
	if(withinTerrain(x,y)){
		return grid[translateTerrainXToGridX(x)][translateTerrainYToGridY(y)];
	}
	else{
		return defaultState;
	}
}


void Grid::setStateOfCoord(int x, int y, string s){
	if(!buffering){
		if(vectorContains(validIds, s)){
			grid[translateTerrainXToGridX(x)][translateTerrainYToGridY(y)] = s;
		}
		else{
			throw new runtime_error("Attempting to set square to an invalid state");	
		}
	}
	else{
		curChangeBuffer.push_back(PointSetBuffer(Pair(x,y), s));
	}
}

void Grid::setAllToDefaultState(){
	for(int x = 0; x < translateTerrainXToGridX(xRange.getHigh()); x++){
		for(int y = 0; y < translateTerrainYToGridY(yRange.getHigh()); y++){
			grid[x][y] = defaultState.c_str();
		}
	}
}

bool Grid::withinTerrain(int x, int y){
	Pair coord(x,y);
	return coord.getFirst() <= xRange.getHigh() && coord.getFirst() >= xRange.getLow() && coord.getSecond() <= yRange.getHigh() && coord.getSecond() >= yRange.getLow();
}

int Grid::translateTerrainXToGridX(int x){
	return ABS(x - xRange.getLow());
}

int Grid::translateTerrainYToGridY(int y){
	return ABS(y - yRange.getLow());
}

PairList * Grid::getPairListForState(string statestr){
	if(vectorContains(validIds, statestr)){
		PairList *res = new PairList();
		for(int x = 0; x < translateTerrainXToGridX(xRange.getHigh()); x++){
			for(int y = 0; y < translateTerrainYToGridY(yRange.getHigh()); y++){
				if(grid[x][y] == statestr){
					res->addPair(x, y);
				}
			}
		}
	}
	else{
		throw new runtime_error("Requested pair list for an invalid state. Silly.");
	}	
}

void Grid::setXRange(Range r){
	xRange = r;
}

void Grid::setYRange(Range r){
	yRange = r;
}

// Returns a shallow copy
Range Grid::getXRange(){
	return xRange;
}

//Returns a shallow copy
Range Grid::getYRange(){
	return yRange;
}


void Grid::startChangeBuffering(){
	buffering = true;
	curChangeBuffer.clear();
}

void Grid::applyChangeBuffer(){
	buffering = false;
	for(int i = 0; i < curChangeBuffer.size(); i++){
		PointSetBuffer b = curChangeBuffer.at(i);
		setStateOfCoord(b.coord.getFirst(), b.coord.getSecond(), b.id);
	}
	curChangeBuffer.clear();
}

Grid::~Grid(){
	delete[] grid;
}
