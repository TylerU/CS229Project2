#include "ArgumentParsing.h"
#include <stdio.h>

int ArgumentParser::getCurIntArg(){
	int i;
	int res = sscanf(argv[curArg], "%d", &i);
	if(res != 1) throw new runtime_error("Unable to read int where expecting integer value");
	return i;
}

Range ArgumentParser::getCurRangeArg(){
	int low = 0, high = 0;
	int res = sscanf(argv[curArg], "%d..%d", &low, &high);
	if(res != 2) throw new runtime_error("Invalid formatting in range argument");
	return Range(low, high);
}

void ArgumentParser::getAndStoreRangeArg(Range *target){
	curArg++;
	Range res = getCurRangeArg();
	target->setHigh(res.getHigh());
	target->setLow(res.getLow());
}

string ArgumentParser::getCurArg(){
	string thisArg(*(argv+curArg));
	return thisArg;
}

void ArgumentParser::handleArg(){
	string thisArg = getCurArg();
	if(thisArg == "-f"){
		opts->setOutputType(FILEOUT);
	}
	else if(thisArg == "-g"){
		curArg++;
		int gen = getCurIntArg();
		opts->setOutputGeneration(gen);
	}
	else if(thisArg == "-tx"){
		getAndStoreRangeArg(&opts->terrainX);
	}
	else if(thisArg == "-ty"){
		getAndStoreRangeArg(&opts->terrainY);
	}
	else if(thisArg == "-h"){
		opts->showOnlyHelpScreen();
	}
	else if(thisArg == "-v"){
		opts->setOutputType(VISUALOUT);
	}
	else if(thisArg == "-wx"){
		getAndStoreRangeArg(&opts->windowX);
	}
	else if(thisArg == "-wy"){
		getAndStoreRangeArg(&opts->windowY);
	}
	else if(thisArg == "-s"){
		curArg++;
		int size = getCurIntArg();
		opts->setBlockSize(size);
	}
	else if(thisArg == "-c"){
		opts->setShowControls();
	}
	else{
		throw new runtime_error("Unexpected argument encountered. I don't know what " + string(argv[curArg]) + " is");
	}
}

ArgumentParser::ArgumentParser(int _argc, char *_argv[]){
	argc = _argc;
	argv = _argv;
	curArg = 0;
}

void ArgumentParser::setOptions(SimOptions *options){
	if(options == NULL){
		throw new runtime_error("Options object for arg parser must be non-null");
	}
	opts = options;
}

void ArgumentParser::parse(){
	if(opts == NULL) throw new runtime_error("Invalid action. You cannot parse command line arguments before setting a valid options target object. This design is terrible");
	string fileName = getFileNameOrEmptyString();
	if(fileName == ""){
		curArg = 1;
	}
	else{
		curArg = 2;
	}

	for(; curArg < argc; curArg++){
		handleArg();
	}
}

string ArgumentParser::getFileNameOrEmptyString(){
	if(argc > 1){
		char *firstArg = *(argv+1);
		if(*firstArg == '-'){
			return string("");
		}
		else{
			return string(firstArg);
		}
	}
	else{
		return string("");
	}
}