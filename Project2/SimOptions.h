#ifndef SIM_OPTIONS_H
#define SIM_OPTIONS_H
#include <string>
#include <vector>
#include <sstream>

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
	IntContainer(int _i){
		i = _i;
	}
	void setInt(int _i){
		i = _i;
	}
	int getInt(){
		return i;
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


class Pair{private:
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
	const vector<Pair> getPairVector(){
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


class SimOptions{
private:
	string simulationId;
public:
	SimOptions() : name("") {

	}
	StringContainer name;
	Range terrainX;
	Range terrainY;
	Range windowX;
	Range windowY;

	void setSimType(string id){
		simulationId = id;
	}
	string getSimType(){
		return simulationId;
	}
};

#endif