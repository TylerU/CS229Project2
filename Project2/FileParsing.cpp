#include "SimOptions.h"
#include "FileParsing.h"
#include <sstream>



ParserCreator::ParserCreator(SimOptions *options) : opts(options){
	parser = NULL;
}

StructElementParser *ParserCreator::getParser(){
	if(!parser){
		parser = new StructElementParser(new map<string, ElementParser*>());
		addParserOptions();
		return parser;
	}
	else{
		return parser;
	}
}

void ParserCreator::addParserOptions(){
	StringElementParser *nameParser = new StringElementParser(&opts->name);

	RangeElementParser *terrainXParser = new RangeElementParser(&opts->terrainX);
	RangeElementParser *terrainYParser = new RangeElementParser(&opts->terrainY);
	RangeElementParser *windowXParser = new RangeElementParser(&opts->windowX);
	RangeElementParser *windowYParser = new RangeElementParser(&opts->windowY);
	
	map<string, ElementParser*> *terrainStruct = new map<string, ElementParser*>();
	terrainStruct->insert(pair<string, ElementParser*>("Xrange", terrainXParser));
	terrainStruct->insert(pair<string, ElementParser*>("Yrange", terrainYParser));
	StructElementParser *terrainParser = new StructElementParser(terrainStruct);

	map<string, ElementParser*> *windowStruct = new map<string, ElementParser*>();
	windowStruct->insert(pair<string, ElementParser*>("Xrange", windowXParser));
	windowStruct->insert(pair<string, ElementParser*>("Yrange", windowYParser));
	StructElementParser *windowParser = new StructElementParser(windowStruct);



	map<string, ElementParser*> *charsStruct = new map<string, ElementParser*>();
	vector<string> validIds = opts->getValidIdentifiers();
	for(int i = 0; i < validIds.size(); i++){
			charsStruct->insert(pair<string, ElementParser*>(validIds[i], new IntElementParser(opts->getDisplayInfoObj(validIds[i])->ascii)));
	}
	StructElementParser *charsParser = new StructElementParser(charsStruct);

	map<string, ElementParser*> *colorsStruct = new map<string, ElementParser*>();
	for(int i = 0; i < validIds.size(); i++){
			colorsStruct->insert(pair<string, ElementParser*>(validIds[i], new TripleElementParser(opts->getDisplayInfoObj(validIds[i])->color)));
	}
	StructElementParser *colorsParser = new StructElementParser(colorsStruct);

	map<string, ElementParser*> *initialStruct = new map<string, ElementParser*>();
	for(int i = 0; i < validIds.size(); i++){
		initialStruct->insert(pair<string, ElementParser*>(validIds[i], new PairsElementParser(opts->getInitialList(validIds[i]))));
	}
	StructElementParser *initialParser = new StructElementParser(initialStruct);

	map<string, ElementParser*> *mainStruct = new map<string, ElementParser*>();
	mainStruct->insert(pair<string, ElementParser*>("Name", nameParser));
	mainStruct->insert(pair<string, ElementParser*>("Terrain", terrainParser));
	mainStruct->insert(pair<string, ElementParser*>("Window", windowParser));
	mainStruct->insert(pair<string, ElementParser*>("Chars", charsParser));
	mainStruct->insert(pair<string, ElementParser*>("Colors", colorsParser));
	mainStruct->insert(pair<string, ElementParser*>("Initial", initialParser));

	parser = new StructElementParser(mainStruct);
}


FileParser::FileParser(string file_n){
	if(file_n == ""){
		in = stdin;
		file_name = string("stdin");
	}
	else{
		in = fopen(file_n.c_str(), "r");
		file_name = file_n;
	}
	options = NULL;
	myParser = NULL;
	if(!in){
		throw new runtime_error("Unable to open the given file");
	}
}

void FileParser::readFile(){
	if(options == NULL) throw new runtime_error("Invalid state. You cannot parse the input file before setting a valid options object");
	if(myParser == NULL) throw new runtime_error("Invalid state. You cannot parse the input file before setting a valid parser object");
	StructElementParser *baseParser = myParser->getParser();//Does this work?
	readOptionsFile(*baseParser);
	freeParser(baseParser);
}

void FileParser::freeParser(StructElementParser *baseParser){
	delete baseParser;
}

string FileParser::getId(){
	StringContainer sc("");
	SimTypeParser tp(&sc);
	tp.read(in);
	return sc.getString();
}


void FileParser::readOptionsFile(StructElementParser& outerParser){
	outerParser.read(in);
};

void IntElementParser::read(FILE *inf){
	setInFile(inf);
	consumeCommentsAndWhitespace();
	int i;
	int res = fscanf(in, "%d", &i);
	if(res != 1){
		throw new runtime_error("Unable to read int");
	}
	ensureNextValidChar(';');
	consumeCommentsAndWhitespace();
	dest->setInt(i);
}

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
		return NULL;
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
	dest->setTriple(trip[0], trip[1], trip[2]);
}



vector<int> PairsElementParser::getSinglePair(){
	vector<int> vec = getVector(2);
	return vec;
}

vector< vector<int> > PairsElementParser::getPairs(){
	vector< vector<int> > vec;
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
	vector< vector<int> > pairs = getPairs();
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
	while((c = fgetc(in)) != '"' && c != EOF){
		ss << (char) c;
	}
	if(c==EOF){
		throw new runtime_error("Unexpected EOF encountered");
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
	
	if(c == EOF){
		throw new runtime_error("Unexpected EOF encountered");
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
	if(c == EOF){
		throw new runtime_error("Unexpected EOF encountered");
	}
}

void ElementParser::getUntilWhitespaceOrMaxLen(char buffer[], int max){
	int c;
	int cur_pos = 0;
	for(c = fgetc(in); c != EOF && !isspace(c) && cur_pos < max-1; c=fgetc(in), cur_pos++){
		buffer[cur_pos] = c;
	}
	if(c == EOF){
		throw new runtime_error("Unexpected EOF encountered");
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
	
	if(c == EOF){
		throw new runtime_error("Unexpected EOF encountered");
	}

	if(c==find || isspace(c)){
		ungetc(c, in);
	}
	ss >> s;
	return s;
}
