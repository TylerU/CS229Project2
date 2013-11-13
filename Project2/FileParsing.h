#ifndef FILE_PARSING_H
#define FILE_PARSING_H

#include <stdio.h>
#include <ctype.h>
#include <string>
#include "SimOptions.h"
#include <sstream>
#include <vector>
#include <map>

using namespace std;



class ElementParser{
protected:
	FILE* in;
protected:
	void consumeCommentsAndWhitespace();
	char getNextValidChar();
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

class SimTypeParser : public ElementParser{
private:
	StringContainer *dest;
public:
	SimTypeParser(StringContainer *destination) : ElementParser() {
		dest = destination;
	}
	virtual void read(FILE *inf);
};

class StringElementParser : public ElementParser{
private:
	StringContainer *dest;
private:
	string getStringValue();
public:
	StringElementParser(StringContainer *destination): ElementParser(){
		dest = destination;
	}
	virtual void read(FILE *inf);
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
	virtual void read(FILE *inf);
};

class VectorElementParser : public ElementParser{
protected:
	vector<int> getVector(int size);
public:
	VectorElementParser(): ElementParser(){
	}
	virtual virtual void read(FILE *inf) =0;
};

class PairsElementParser : public VectorElementParser{
private:
	PairList *dest;
private:
	vector<vector<int>> getPairs();
	vector<int> getSinglePair();
public:
	PairsElementParser(PairList *destination): VectorElementParser(){
		dest = destination;
	}
	virtual void read(FILE *inf);
};

class TripleElementParser : public VectorElementParser{
private:
	Triple *dest;
private:
	vector<int> getTriple();
public:
	TripleElementParser(Triple *destination): VectorElementParser(){
		dest = destination;
	}
	virtual void read(FILE *inf);
};


class StructElementParser : public ElementParser{
private:
	map<string, ElementParser*> *elements;
private:
	ElementParser* getElement(string key);
public:
	StructElementParser(map<string, ElementParser*> *elems): ElementParser(){
		elements = elems;
	}
	virtual void read(FILE *inf);
};

class FileParser{
public:
	FileParser(string file_name, SimOptions& opts);
private:
	FILE *in;
	SimOptions &options;
	string file_name;
private:
	void readFile();
	string getId();
	StructElementParser *createParser(string id);
	void readOptionsFile(StructElementParser& outerParser);
};

#endif