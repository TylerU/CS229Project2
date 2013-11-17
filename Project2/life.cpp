#include "SimOptions.h"
#include "FileParsing.h"
#include "ArgumentParsing.h"
#include "Grid.h"


bool ensureCorrect(SimOptions *opts){
	if(	opts->name.getString() != "Figure 3 example" || 
		opts->terrainX.getHigh() != 6 ||
		opts->terrainX.getLow() != -6 ||
		opts->terrainY.getHigh() != 3 ||
		opts->terrainY.getLow() != -3 ||
		opts->getDisplayInfoObj("Alive")->ascii->getInt() != 64 ||
		opts->getDisplayInfoObj("Alive")->color->getFirst() != 255 ||
		opts->getDisplayInfoObj("Alive")->color->getSecond() != 255 ||
		opts->getDisplayInfoObj("Alive")->color->getThird() != 255 ||
		opts->getDisplayInfoObj("Dead")->ascii->getInt() != 126 ||
		opts->getDisplayInfoObj("Dead")->color->getFirst() != 64 ||
		opts->getDisplayInfoObj("Dead")->color->getSecond() != 64 ||
		opts->getDisplayInfoObj("Dead")->color->getThird() != 64 //||
		/*opts->getInitialList("Alive")->getPairVector()[0].getFirst() != -2 ||
		opts->getInitialList("Alive")->getPairVector()[0].getSecond() != 1 ||
		opts->getInitialList("Alive")->getPairVector()[5].getFirst() != 4 ||
		opts->getInitialList("Alive")->getPairVector()[5].getSecond() != -1 ||
		opts->getInitialList("Dead")->getPairVector().size() != 0 */
	){
		return false;
	}
	return true;
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

		Grid g(options->terrainX, options->terrainY, options);
		for(int y = options->windowY.getLow(); y <= options->windowY.getHigh(); y++){
			for(int x = options->windowX.getLow(); x <= options->windowX.getHigh(); x++){
				printf("%c", options->getCharforState(g.getStateOfCoord(x,y)));
			}
			printf("\n");
		}
	}catch (runtime_error e){
		fprintf(stderr, "Error encountered: %s\n", e.what());
	}

	system("pause");
	return 0;
}