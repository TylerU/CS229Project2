#ifndef ARGPARSER_H
#define ARGPARSER_H
#include "SimOptions.h"

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

class LifeGUIArgumentParser : public ArgumentParser{
protected:
	virtual void handleArg();
	LifeGUISimOptions *getSimOptions();
public:
	LifeGUIArgumentParser(int _argc, char *_argv[]) : ArgumentParser(_argc, _argv) {}

};
#endif