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
	virtual void handleArg();
public:
	ArgumentParser(int _argc, char *_argv[]);
	void setOptions(SimOptions *options);
	void parse();
	string getFileNameOrEmptyString();
};

class LifeArgumentParser : public ArgumentParser{

};