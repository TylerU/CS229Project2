#ifndef SIM_OPTIONS_H
#define SIM_OPTIONS_H
#include <string>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

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
	int getFirst(){
		return first;
	}
	int getSecond(){
		return second;
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
	vector<Pair> getPairVector(){
		return pairs;
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
public:
	SimOptions() : name("") {}
	StringContainer name;
	Range terrainX;
	Range terrainY;
	Range windowX;
	Range windowY;

	StateDisplayInfo *getDisplayInfoObj(string state){
		return displayInfo[state];
	}

	void setSimType(string id){
		simulationId = id;
	}
	string getSimType(){
		return simulationId;
	}
	PairList *getInitialList(string id){
		return initial[id];
	}
	virtual vector<string> getValidIdentifiers()=0;
	virtual string getDefaultStateString()=0;
};

class LifeSimOptions : public SimOptions{
public:
	LifeSimOptions(){
		displayInfo.emplace("Alive", new StateDisplayInfo('@', 0,0,0));
		displayInfo.emplace("Dead", new StateDisplayInfo('-', 255,255,255));
		initial.emplace("Alive", new PairList());
		initial.emplace("Dead", new PairList());
	}
	virtual vector<string> getValidIdentifiers(){
		vector<string> result;
		result.push_back("Alive");
		result.push_back("Dead");
		return result;
	}
	virtual string getDefaultStateString(){
		return "Dead";
	}
};
#endif