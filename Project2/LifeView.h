#ifndef LIFEVIEW_H
#define LIFEVIEW_H
#include "SimController.h"
#include "SimOptions.h"
#include <stdexcept>
#include <stdio.h>

void write_file_to_file(FILE *src, FILE *dest);

void append_file_to_file(string src, FILE *dest_file);

class BasicCmdLineView{
protected:
	SimOptions *opts;
	SimController *controller;
public:
	BasicCmdLineView(SimOptions *options, SimController *contr);
	virtual void writeout(FILE* out)=0;
};

class FileFormatOutput : public BasicCmdLineView{
public:
	FileFormatOutput(SimOptions *options, SimController *contr) : BasicCmdLineView(options, contr) {}

	virtual void writeout(FILE* out);
};

class VisualOutput : public BasicCmdLineView{
public:
	VisualOutput(SimOptions *options, SimController *contr) : BasicCmdLineView(options, contr) {}

	virtual void writeout(FILE* out);
};

class LifeHelpOutput : public BasicCmdLineView{
public:
	LifeHelpOutput(SimOptions *options, SimController *contr) : BasicCmdLineView(options, contr) {}

	virtual void writeout(FILE* out);
};

#endif