#include "SimOptions.h"
#include "FileParsing.h"
#include "ArgumentParsing.h"
#include "Grid.h"
#include "SimController.h"
#include "LifeView.h"
#include "InitHelpers.h"
#include <stdexcept>



BasicCmdLineView *getView(SimOptions *options, SimController *sc){
	if(!options->getShowHelp()){
		if(options->getOutputType() == FILEOUT){
			return new FileFormatOutput(options, sc);
		}
		else {
			return new VisualOutput(options, sc);
		}
	}
	else{
		return new LifeHelpOutput(options, sc);
	}
}



int main(int argc, char* argv[]){
	try{
		LifeObjects objs = initLife(argc, argv);
		objs.sc->simGenerations(objs.options->getOutputGeneration());
		BasicCmdLineView *view = getView(objs.options, objs.sc);

		view->writeout(stdout);
	}catch (runtime_error e){
		fprintf(stderr, "Error encountered: %s\n", e.what());
	}

	system("pause");
	return 0;
}
