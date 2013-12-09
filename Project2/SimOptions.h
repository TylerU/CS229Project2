#ifndef SIM_OPTIONS_H
#define SIM_OPTIONS_H
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <stdexcept>

using namespace std;

enum OutputType{
	FILEOUT,
	VISUALOUT
};

class StringContainer{
private:
	string str;
public:
	StringContainer(string s){
		str = s;
	}
	string getString(){
		return str;
	}
	void setString(string s){
		str = s;
	}
};

class IntContainer{
private:
	int i;
public:
	IntContainer(int ii){
		i = ii;
	}
	int getInt(){
		return i;
	}
	void setInt(int ii){
		i = ii;
	}
};

class Triple{
private:
	int first;
	int second;
	int third;
public:
	Triple(int f, int s, int t){
		first = f;
		second = s;
		third = t;
	}
	void setTriple(int f, int s, int t){
		first = f;
		second = s;
		third = t;
	}
	int getFirst(){
		return first;
	}
	int getSecond(){
		return second;
	}
	int getThird(){
		return third;
	}
};


class Pair{
private:
	int first;
	int second;
public:
	Pair(int f, int s){
		first = f;
		second = s;
	}
	void setPair(int f, int s){
		first = f;
		second = s;
	}
	int getFirst() const{
		return first;
	}
	int getSecond() const{
		return second;
	}
	bool operator==(const Pair& p){
		return getFirst() == p.getFirst() && getSecond() == p.getSecond();
	}
};

class PairList{
private:
	vector<Pair> pairs;
public:
	PairList(){
	}
	void addPair(int f, int s){
		pairs.push_back(Pair(f,s));
	}
	//Too lazy to write more getters.
	const vector<Pair> *getPairVector() const{
		return &pairs;
	}
	bool contains(Pair p){
		for(int i = 0; i < pairs.size(); i++){
			if(p == pairs.at(i)){
				return true;
			}
		}
		return false;
	}
	void remove(Pair p){
		for(int i = 0; i < pairs.size(); i++){
			if(p == pairs.at(i)){
				pairs.erase(pairs.begin() + i);
			}
		}
	}
};

class Range{
private:
	int high;
	int low;
public:
	Range(){
		high = 0;
		low = 0;
	}
	Range(int l, int h){
		if(l>h) throw new runtime_error("Invalid range. Low is larger than high");
		high = h;
		low = l;
	}
	inline int getHigh(){ return high; }
	inline int getLow(){ return low; }
	inline void setHigh(int h){ high = h; }
	inline void setLow(int l){ low = l; }
	inline string toString(){
		stringstream ss;
		ss << low << ".." << high;
		return ss.str();
	}
};



class StateDisplayInfo{
public:
	IntContainer *ascii;
	Triple *color;
	StateDisplayInfo(int defaultChar, int c1, int c2, int c3){
		ascii = new IntContainer(defaultChar);
		color = new Triple(c1, c2, c3);
	}
	~StateDisplayInfo(){
		delete ascii;
		delete color;
	}
};

class SimOptions{
protected:
	map<string, StateDisplayInfo *> displayInfo;
	map<string, PairList *> initial;
	string simulationId;
	OutputType outputType;
	int outputGeneration;
	bool showHelp;
	int blockSize;
	bool controls;

public:
	StringContainer name;
	Range terrainX;
	Range terrainY;
	Range windowX;
	Range windowY;

public:
	SimOptions() : name(""), simulationId("") {
		outputGeneration = 0;
		outputType = VISUALOUT;
		showHelp = false;
		blockSize = 10;
		controls = false;
	}

	void setShowControls(){ controls = true; }

	bool showControls(){ return controls; }

	int getBlockSize(){ return blockSize; }

	void setBlockSize(int s) { blockSize = s; }

	bool drawLines(){ return blockSize >= 4; }

	StateDisplayInfo *getDisplayInfoObj (string state) {
		return displayInfo[state];
	}

	void setSimType(string id){
		simulationId = id;
	}
	
	string getSimType() const{
		return simulationId;
	}
	
	PairList *getInitialList(string id) {
		return initial[id];
	}
	
	void setOutputType(OutputType type){
		outputType = type;
	}
	
	OutputType getOutputType() const{
		return outputType;
	}
	
	void setOutputGeneration(int gen){
		outputGeneration = gen;
	}
	
	int getOutputGeneration() const{
		return outputGeneration;
	}
	
	void showOnlyHelpScreen(){
		showHelp = true;
	}

	bool getShowHelp(){
		return showHelp;
	}

	/*
		Set any defaults that are reliant upon other values being set. Namely, the window ranges.
		This is called after ALL argument setting is done. 
	*/
	virtual void resolveDefaults(){
		if(windowX.getHigh() == 0 && windowX.getLow() == 0){
			windowX.setHigh(terrainX.getHigh());
			windowX.setLow(terrainX.getLow());
		}
		if(windowY.getHigh() == 0 && windowY.getLow() == 0){
			windowY.setHigh(terrainY.getHigh());
			windowY.setLow(terrainY.getLow());
		}
	}

	char getCharforState(string s) {
		return (char) displayInfo[s]->ascii->getInt();
	}

	Triple getColorForState(string s) {
		return *(displayInfo[s]->color);
	}

	virtual vector<string> getValidIdentifiers() const=0;
	virtual string getDefaultStateString() const=0;
};

class LifeSimOptions : public SimOptions{
public:
	LifeSimOptions(){
		displayInfo.insert(pair<string, StateDisplayInfo*>("Alive", new StateDisplayInfo('@', 0,0,0)));
		displayInfo.insert(pair<string, StateDisplayInfo *>("Dead", new StateDisplayInfo('-', 255,255,255)));
		initial.insert(pair<string, PairList *>("Alive", new PairList()));
		initial.insert(pair<string, PairList *>("Dead", new PairList()));
	}
	virtual vector<string> getValidIdentifiers() const{
		vector<string> result;
		result.push_back("Alive");
		result.push_back("Dead");
		return result;
	}
	virtual string getDefaultStateString() const{
		return "Dead";
	}
};

class WireWorldSimOptions : public SimOptions{
public:
	WireWorldSimOptions(){
		displayInfo.insert(pair<string, StateDisplayInfo*>("Empty", new StateDisplayInfo(32, 64, 64, 64)));
		displayInfo.insert(pair<string, StateDisplayInfo *>("Head", new StateDisplayInfo(35, 255, 64, 64)));
		displayInfo.insert(pair<string, StateDisplayInfo *>("Tail", new StateDisplayInfo(43, 255, 64, 255)));
		displayInfo.insert(pair<string, StateDisplayInfo *>("Wire", new StateDisplayInfo(46, 64, 64, 255)));
		initial.insert(pair<string, PairList *>("Empty", new PairList()));
		initial.insert(pair<string, PairList *>("Head", new PairList()));
		initial.insert(pair<string, PairList *>("Tail", new PairList()));
		initial.insert(pair<string, PairList *>("Wire", new PairList()));
	}
	virtual vector<string> getValidIdentifiers() const{
		vector<string> result;
		result.push_back("Empty");
		result.push_back("Head");
		result.push_back("Tail");
		result.push_back("Wire");
		return result;
	}
	virtual string getDefaultStateString() const{
		return "Empty";
	}
};

class ElementarySimOptions : public SimOptions{
public:
	IntContainer ruleContainer;
	ElementarySimOptions() : ruleContainer(30){
		displayInfo.insert(pair<string, StateDisplayInfo*>("Zero", new StateDisplayInfo(46, 255, 255, 255)));
		displayInfo.insert(pair<string, StateDisplayInfo *>("One", new StateDisplayInfo(35, 64, 64, 64)));
		initial.insert(pair<string, PairList *>("Zero", new PairList()));
		initial.insert(pair<string, PairList *>("One", new PairList()));
	}
	virtual vector<string> getValidIdentifiers() const{
		vector<string> result;
		result.push_back("Zero");
		result.push_back("One");
		return result;
	}
	virtual string getDefaultStateString() const{
		return "Zero";
	}
};

#endif
