#ifndef ARGPARSER_H
#define ARGPARSER_H
#include "SimOptions.h"
#include <stdexcept>

/*
	Parses command line arguments into the simOptions settings object
*/
class ArgumentParser{
protected:
	int argc;
	char **argv;
	int curArg;
	SimOptions *opts;
	int getCurIntArg();
	Range getCurRangeArg();
	void getAndStoreRangeArg(Range *target);
	string getCurArg();
	virtual void handleArg();
public:
	ArgumentParser(int _argc, char *_argv[]);
	void setOptions(SimOptions *options);
	void parse();
	string getFileNameOrEmptyString();
};

#endif