#ifndef FILE_PARSING_H
#define FILE_PARSING_H

#include <stdio.h>
#include <ctype.h>
#include <string>
#include "SimOptions.h"
#include <sstream>
#include <vector>
#include <map>
#include <stdexcept>

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
	virtual ~ElementParser(){
	}
};

class IntElementParser : public ElementParser {
private:
	IntContainer *dest;
public:
	IntElementParser(IntContainer *destination) : ElementParser() {
		dest = destination;
	}
	virtual void read(FILE *inf);
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
	virtual void read(FILE *inf) =0;
};

class PairsElementParser : public VectorElementParser{
private:
	PairList *dest;
private:
	vector< vector<int> > getPairs();
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
public:
	StructElementParser(map<string, ElementParser*> *elems): ElementParser(){
		elements = elems;
	}
	ElementParser* getElement(string key);
	void addParserKeyword(string key, ElementParser *parser){
		elements->insert(pair<string, ElementParser*>(key, parser));
	}
	virtual void read(FILE *inf);
	virtual ~StructElementParser(){
		for(std::map<string,ElementParser*>::iterator iter = elements->begin(); iter != elements->end(); ++iter)
		{
			delete iter->second;
		}
		delete elements;
	}
};


class ParserCreator{
protected:
	StructElementParser *parser;
	SimOptions* opts;
	virtual void addParserOptions();
public:
	ParserCreator(SimOptions* options);
	
	StructElementParser *getParser();
};

class FileParser{
public:
	FileParser(string file_name);
	string getId();
	void setSimOptions(SimOptions *opts){
		options = opts;
	}
	void setParser(ParserCreator *parser){
		myParser = parser;
	}
	void readFile();
private:
	FILE *in;
	SimOptions *options;
	string file_name;
	ParserCreator *myParser;
private:
	void freeParser(StructElementParser *baseParser);
	void readOptionsFile(StructElementParser& outerParser);
};

#endif