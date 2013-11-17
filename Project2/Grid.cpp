#include "Grid.h"


Grid::Grid(const SimOptions *opts){
	state = new map<string, PairList *>();
	xRange = opts->terrainX;
	yRange = opts->terrainY;
	buffering = false;
	defaultState = opts->getDefaultStateString();
	vector<string> ids = opts->getValidIdentifiers();
	for(int i = 0; i < ids.size(); i++){
		if(ids[i] != defaultState){
			addStartState(ids[i], opts->getInitialList(ids[i]));
		}
	}
}

void Grid::addStartState(string id, PairList *list){
	PairList *myList = new PairList();
	state->emplace(id, myList);
	
	const vector<Pair> *pairList = list->getPairVector();
	for(int i = 0; i < pairList->size(); i++){
		Pair his = pairList->at(i);
		addToState(his, id);
	}

}


string Grid::getStateOfCoord(int x, int y){
	Pair given(x,y);
	for(std::map<string, PairList*>::iterator iter = state->begin(); iter != state->end(); ++iter)
	{
		string curState =  iter->first;
		PairList *list = iter->second;
		if(list->contains(given)){
			return curState;
		}
	}
	return defaultState;
}


void Grid::setStateOfCoord(int x, int y, string s){
	if(!buffering){
		removeFromAllStates(Pair(x,y));
		if(s != defaultState){
			addToState(Pair(x,y), s);
		}
	}
	else{
		curChangeBuffer.push_back(PointSetBuffer(Pair(x,y), s));
	}
}

void Grid::addToState(Pair coord, string s){
	try{
		if(coord.getFirst() <= xRange.getHigh() && coord.getFirst() >= xRange.getLow() && coord.getSecond() <= yRange.getHigh() && coord.getSecond() >= yRange.getLow()){
			state->at(s)->addPair(coord.getFirst(), coord.getSecond());
		}
		else{
			//Ignore entries outside our terrain range
		}
	}
	catch(...){
		throw new runtime_error("Attempting to set state of a coordinate to an invalid state");
	}
}

void Grid::removeFromAllStates(Pair coord){
	for(std::map<string, PairList*>::iterator iter = state->begin(); iter != state->end(); ++iter)
	{
		string curState =  iter->first;
		PairList *list = iter->second;
		list->remove(coord);
	}
}

const PairList const* Grid::getPairListForState(string statestr){
	try{
		return state->at(statestr);
	}
	catch(...){
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
	delete state;
}