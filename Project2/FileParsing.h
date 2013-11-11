#ifndef FILE_PARSING_H
#define FILE_PARSING_H

#include <stdio.h>
#include <ctype.h>
#include <string>
#include "SimOptions.cpp"
#include <sstream>
#include <vector>
#include <map>

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

class ElementParser{
protected:
	FILE* in;
protected:
	void consumeCommentsAndWhitespace();
	//vector<int> getTriple();
	//vector<vector<int>> getPairs();
	void ensureNextValidChar(char c);
	void ensureNextChar(char c);
	void consumeWhitespace();
	bool isEndOfLine(int c);
	void consumeUntilEndline();
	void getUntilWhitespaceOrMaxLen(char buffer[], int max);
	string getUntilCharOrWhitespace(char find);
public:
	ElementParser(FILE *inf = NULL){
		in = inf;
	}
	inline void setInFile(FILE *inf){
		in = inf;
	}
	virtual void read(FILE *inf) =0;
};

class StringElementParser : public ElementParser{
private:
	string *dest;
private:
	string getStringValue();
public:
	StringElementParser(string *destination): ElementParser(){
		dest = destination;
	}
	void read(FILE *inf);
};

class RangeElementParser : public ElementParser{
private:
	Range *dest;
private:
	Range getRangeValue();
public:
	RangeElementParser(Range *destination): ElementParser(){
		dest = destination;
	}
	void read(FILE *inf);
};

class VectorElementParser : public ElementParser{
protected:
	vector<int> getVector(int size);
public:
	VectorElementParser(): ElementParser(){
	}
	virtual void read(FILE *inf) =0;
};

class PairsElementParser : public VectorElementParser{
private:
	vector<vector<int>> *dest;
private:
	vector<vector<int>> getPairs();
	vector<int> getSinglePair();
public:
	PairsElementParser(vector<vector<int>> *destination): VectorElementParser(){
		dest = destination;
	}
	void read(FILE *inf);
};

class TripleElementParser : public VectorElementParser{
private:
	vector<vector<int>> *dest;
private:
	vector<int> getTriple();
public:
	TripleElementParser(vector<vector<int>> *destination): VectorElementParser(){
		dest = destination;
	}
	void read(FILE *inf);
};


class StructElementParser : public ElementParser{
private:
	map<string, ElementParser*> elements;
private:
public:
	StructElementParser(map<string, ElementParser*> elems): ElementParser(){
		elements = elems;
	}
	void read(FILE *inf);
}

class FileParser{
public:
	FileParser(string file_name, SimOptions& opts);
private:
	FILE *in;
	SimOptions &options;
	string file_name;
private:
	void readOptionsFile();
};

#endif