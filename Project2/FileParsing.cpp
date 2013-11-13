#include "SimOptions.h"
#include "FileParsing.h"
#include <sstream>




FileParser::FileParser(string file_n, SimOptions& opts) : options(opts){
	in = fopen(file_n.c_str(), "r");
	file_name = file_n;
	if(!in){
		throw new runtime_error("Unable to open the given file");
	}
	readFile();
}

void FileParser::readFile(){
	string id = getId();
	options.setSimType(id);
	StructElementParser *baseParser = createParser(id);//Does this work?
	readOptionsFile(*baseParser);
}

string FileParser::getId(){
	StringContainer sc("");
	SimTypeParser tp(&sc);
	tp.read(in);
	return sc.getString();
}

StructElementParser *FileParser::createParser(string id){
	StringElementParser *nameParser = new StringElementParser(&options.name);

	RangeElementParser *terrainXParser = new RangeElementParser(&options.terrainX);
	RangeElementParser *terrainYParser = new RangeElementParser(&options.terrainY);
	RangeElementParser *windowXParser = new RangeElementParser(&options.windowX);
	RangeElementParser *windowYParser = new RangeElementParser(&options.windowY);
	
	map<string, ElementParser*> *terrainStruct = new map<string, ElementParser*>();
	terrainStruct->emplace("Xrange", terrainXParser);
	terrainStruct->emplace("Yrange", terrainYParser);
	StructElementParser *terrainParser = new StructElementParser(terrainStruct);

	map<string, ElementParser*> *windowStruct = new map<string, ElementParser*>();
	windowStruct->emplace("Xrange", windowXParser);
	windowStruct->emplace("Yrange", windowYParser);
	StructElementParser *windowParser = new StructElementParser(windowStruct);

	map<string, ElementParser*> *mainStruct = new map<string, ElementParser*>();
	mainStruct->emplace("Name", nameParser);
	mainStruct->emplace("Terrain", terrainParser);
	mainStruct->emplace("Window", windowParser);
	
	StructElementParser *mainParser = new StructElementParser(mainStruct);
	return mainParser;
}

void FileParser::readOptionsFile(StructElementParser& outerParser){
	outerParser.read(in);
};

void SimTypeParser::read(FILE *inf){
	setInFile(inf);
	consumeCommentsAndWhitespace();
	string fileType = getUntilCharOrWhitespace('=');
	ensureNextValidChar('=');
	dest->setString(fileType);
	consumeCommentsAndWhitespace();
}

void StructElementParser::read(FILE *inf){
	setInFile(inf);
	ensureNextValidChar('{');
	while(getNextValidChar() != '}'){
		consumeCommentsAndWhitespace();
		string id = getUntilCharOrWhitespace('=');
		ensureNextValidChar('=');
		consumeCommentsAndWhitespace();
		ElementParser *parser = getElement(id);
		if(parser){
			parser->read(in);
		}
		else{
			throw new runtime_error("Invalid key encountered");
		}
	}
	ensureNextValidChar('}');
	ensureNextValidChar(';');
}

ElementParser *StructElementParser::getElement(string key){
	map<string, ElementParser*>::iterator it = elements->find(key);
	if(it != elements->end()){
		return it->second;
	}
	else{
		return false;
	}
}


vector<int> TripleElementParser::getTriple(){
	vector<int> vec = getVector(3);
	ensureNextValidChar(';');
	return vec;
}

void TripleElementParser::read(FILE* inf){
	setInFile(inf);
	vector<int> trip = getTriple();
	dest->setTriple(trip[0], trip[1], trip[3]);
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
	setInFile(inf);
	vector<vector<int>> pairs = getPairs();
	for(int i = 0; i < pairs.size(); i++){
		dest->addPair(pairs[i][0], pairs[i][1]);
	}
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
	setInFile(inf);
	Range result = getRangeValue();
	dest->setHigh(result.getHigh());
	dest->setLow(result.getLow());
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

char ElementParser::getNextValidChar(){
	consumeCommentsAndWhitespace();
	int c = fgetc(in);
	ungetc(c, in);
	return (char)c;
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
	int c = 0;
	while(isspace(c = fgetc(in)) && c != EOF){
	}
	if(!isspace(c)){
		ungetc(c, in);
	}
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
