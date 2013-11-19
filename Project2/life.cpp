#include "SimOptions.h"
#include "FileParsing.h"
#include "ArgumentParsing.h"
#include "Grid.h"
#include "SimController.h"
#include "LifeView.h"
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
		SimOptions *options = NULL;
		ArgumentParser ap(argc, argv);
		FileParser fp(ap.getFileNameOrEmptyString());
		string type = fp.getId();
		if(type == "Life"){
			fp.setSimOptions(options = new LifeSimOptions());
			ap.setOptions(options);
			fp.setParser(new ParserCreator(options));
			options->setSimType(type);
		}
		fp.readFile();
		ap.parse();
		options->resolveDefaults();

		SimController sc(options, new LifeSimRunner());
		sc.simGenerations(options->getOutputGeneration());
		BasicCmdLineView *view = getView(options, &sc);

		view->writeout(stdout);
	}catch (runtime_error e){
		fprintf(stderr, "Error encountered: %s\n", e.what());
	}

	//system("pause");
	return 0;
}