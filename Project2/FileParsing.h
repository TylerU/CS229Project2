#ifndef FILE_PARSING_H
#define FILE_PARSING_H

#include <stdio.h>
#include <ctype.h>
#include <string>
#include "SimOptions.cpp"
#include <sstream>
#include <vector>

using namespace std;

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

class FileParser{
public:
	FileParser(string file_name, SimOptions& opts);
private:
	FILE *in;
	SimOptions &options;
	string file_name;
private:
	void readOptionsFile();
	void consumeCommentsAndWhitespace();
	string getStringValue();
	vector<int> getTriple();
	vector<int> getVector(int size);
	vector<int> getSinglePair();
	vector<vector<int>> getPairs();
	Range getRangeValue();
	void ensureNextValidChar(char c);
	void ensureNextChar(char c);
	void consumeWhitespace();
	bool isEndOfLine(int c);
	void consumeUntilEndline();
	void getUntilWhitespaceOrMaxLen(char buffer[], int max);
	string getUntilCharOrWhitespace(char find);
};

#endif