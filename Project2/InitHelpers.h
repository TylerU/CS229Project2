#ifndef INIT_HELPERS
#define INIT_HELPERS 
#include "SimOptions.h"
#include "FileParsing.h"
#include "ArgumentParsing.h"
#include "SimController.h"

typedef struct {
	SimOptions *options;
	SimController *sc;
} LifeObjects;

/* Parses command line args, files, and creates the options and sim controller objects */
LifeObjects initLife(int argc, char* argv[]);

#endif