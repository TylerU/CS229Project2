#include "SimOptions.h"
#include "FileParsing.h"
#include <sstream>




FileParser::FileParser(string file_n, SimOptions& opts) : options(opts){
	in = fopen(file_n.c_str(), "r");
	file_name = file_n;
	if(!in){
		throw new runtime_error("Unable to open the given file");
	}
	readOptionsFile();
}

void FileParser::readOptionsFile(){
	consumeCommentsAndWhitespace();
	string fileType = getUntilCharOrWhitespace('=');
	printf("\"%s\"\n", fileType.c_str());
	consumeCommentsAndWhitespace();
	ensureNextChar('=');
	consumeCommentsAndWhitespace();
	ensureNextChar('{');
	consumeCommentsAndWhitespace();
	string id = getUntilCharOrWhitespace('=');
	ensureNextValidChar('=');
	string val = getStringValue();
	printf("\"%s\" = \"%s\"\n", id.c_str(), val.c_str());
	consumeCommentsAndWhitespace();
	id = getUntilCharOrWhitespace('=');
	ensureNextValidChar('=');
	Range rVal = getRangeValue();
	printf("\"%s\" = %s\n", id.c_str(), rVal.toString().c_str());
	consumeCommentsAndWhitespace();
	id = getUntilCharOrWhitespace('=');
	ensureNextValidChar('=');
	rVal = getRangeValue();
	printf("\"%s\" = %s\n", id.c_str(), rVal.toString().c_str());
	consumeCommentsAndWhitespace();
	id = getUntilCharOrWhitespace('=');
	ensureNextValidChar('=');
	vector<int> vec = getTriple();
	printf("\"%s\" = (%d, %d, %d)\n", id.c_str(), vec[0], vec[1], vec[2]);
	consumeCommentsAndWhitespace();
	id = getUntilCharOrWhitespace('=');
	ensureNextValidChar('=');
	vec = getSinglePair();
	ensureNextValidChar(';');
	printf("\"%s\" = (%d, %d)\n", id.c_str(), vec[0], vec[1]);
	consumeCommentsAndWhitespace();
	id = getUntilCharOrWhitespace('=');
	ensureNextValidChar('=');
	vector<vector<int>> pairs = getPairs();
	for(int i = 0; i < pairs.size(); i++){
		vec = pairs[i];
		printf("\"%s\" = (%d, %d)\n", id.c_str(), vec[0], vec[1]);
	}
	
};


void StructElementParser::read(FILE *inf){

}


vector<int> TripleElementParser::getTriple(){
	vector<int> vec = getVector(3);
	ensureNextValidChar(';');
	return vec;
}

void TripleElementParser::read(FILE* inf){

}



vector<int> PairsElementParser::getSinglePair(){
	vector<int> vec = getVector(2);
	return vec;
}

vector<vector<int>> PairsElementParser::getPairs(){
	vector<vector<int>> vec;
	consumeCommentsAndWhitespace();
	char nextDelim = 0;
	while(nextDelim == 0 || (nextDelim = fgetc(in)) == ','){
		nextDelim = 1;
		consumeCommentsAndWhitespace();
		vec.push_back(getSinglePair());
		consumeCommentsAndWhitespace();
	}
	if(nextDelim != ';'){
		stringstream ss;
		ss << "Invalid formatting in pair list. Expected ';'. Encountered '" << nextDelim;
		throw new runtime_error(ss.str());
	}
	return vec;
}

void PairsElementParser::read(FILE* inf){

}

vector<int> VectorElementParser::getVector(int size){
	vector<int> vec;
	consumeCommentsAndWhitespace();
	ensureNextValidChar('(');
	for(int i = 0; i < size; i++){
		consumeCommentsAndWhitespace();
		int val;
		int res = fscanf(in, "%d", &val);
		if(res != 1){
			throw new runtime_error("Invalid formatting in triplet or pair");
		}
		if(i < size-1){
			ensureNextValidChar(',');
		}
		vec.push_back(val);
	}
	ensureNextValidChar(')');
	return vec;
}

Range RangeElementParser::getRangeValue(){
	consumeCommentsAndWhitespace();
	int high, low;
	int result = fscanf(in, "%d", &low);
	if(result != 1){
		throw new runtime_error("Invalid formatting for range value");
	}
	ensureNextValidChar('.');
	ensureNextValidChar('.');
	consumeCommentsAndWhitespace();
	result = fscanf(in, "%d", &high);
	if(result != 1){
		throw new runtime_error("Invalid formatting for range value");
	}
	ensureNextValidChar(';');
	return Range(low, high);
}

void RangeElementParser::read(FILE *inf){

}

string StringElementParser::getStringValue(){
	stringstream ss;
	ensureNextValidChar('"');
	int c;
	while((c = fgetc(in)) != '"'){
		ss << (char) c;
	}
	ensureNextValidChar(';');
	return ss.str();
}
void StringElementParser::read(FILE* inf){
	setInFile(inf);
	//Probably broken
	*dest = getStringValue();
}

void ElementParser::ensureNextValidChar(char c){
	consumeCommentsAndWhitespace();
	ensureNextChar(c);
}

void ElementParser::ensureNextChar(char c){
	int next;
	if((next=fgetc(in)) != c){
		stringstream ss;
		ss << "Unexpected character encountered while parsing the input file";
		ss << ". Expected: '" << c << "'. Encountered: '" << (char)next;

		throw new runtime_error(ss.str());
	}
}

void ElementParser::consumeCommentsAndWhitespace(){
	consumeWhitespace();
	char c = fgetc(in);		
	while(c == '#'){
		consumeUntilEndline();
		consumeWhitespace();
		c = fgetc(in);
	}
	ungetc(c, in);
}

void ElementParser::consumeWhitespace(){
	int c;
	while(isspace(c = fgetc(in))){
	}
	ungetc(c, in);
}


bool ElementParser::isEndOfLine(int c)
{
	int eol = (c == '\r' || c == '\n');
	if (c == '\r')
	{
		c = getc(in);
		if (c != '\n' && c != EOF)
			ungetc(c, in);
	}
	return eol;
}

void ElementParser::consumeUntilEndline(){
	int c;

	for(c = fgetc(in); c != EOF && !isEndOfLine(c); c = fgetc(in)){
	}
}

void ElementParser::getUntilWhitespaceOrMaxLen(char buffer[], int max){
	int c;
	int cur_pos = 0;
	for(c = fgetc(in); c != EOF && !isspace(c) && cur_pos < max-1; c=fgetc(in), cur_pos++){
		buffer[cur_pos] = c;
	}
	if(isspace(c)){
		ungetc(c, in);
	}
	buffer[cur_pos] = '\0';
}

string ElementParser::getUntilCharOrWhitespace(char find){
	int c;
	string s;
	stringstream ss;
	for(c = fgetc(in); c != EOF && c != find && !isspace(c); c=fgetc(in)){
		ss << (char)c;
	}
	if(c==find || isspace(c)){
		ungetc(c, in);
	}
	ss >> s;
	return s;
}
