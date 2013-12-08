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

LifeObjects initLife(int argc, char* argv[]);

#endif