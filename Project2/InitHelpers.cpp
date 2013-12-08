#include "InitHelpers.h"

LifeObjects initLife(int argc, char* argv[]){
		LifeObjects objs;

		SimOptions *options = NULL;
		SimRunner *runner = NULL;
		ArgumentParser ap(argc, argv);
		FileParser fp(ap.getFileNameOrEmptyString());
		string type = fp.getId();
		if(type == "Life"){
			fp.setSimOptions(options = new LifeSimOptions());
			ap.setOptions(options);
			fp.setParser(new ParserCreator(options));
			options->setSimType(type);
			runner = new LifeSimRunner();
		}
		else if(type == "WireWorld"){
			fp.setSimOptions(options = new WireWorldSimOptions());
			ap.setOptions(options);
			fp.setParser(new ParserCreator(options));
			options->setSimType(type);
			runner = new WireWorldSimRunner();
		}
		else if(type == "Elementary"){
			fp.setSimOptions(options = new ElementarySimOptions());
			ap.setOptions(options);
			fp.setParser(new ElementaryParserCreator((ElementarySimOptions*) options));//Messy
			options->setSimType(type);
			runner = new ElementarySimRunner(((ElementarySimOptions*) options)->ruleContainer.getInt());
		}
		else{
			throw new runtime_error("Invalid simulation type in options file");
		}
		fp.readFile();
		ap.parse();
		options->resolveDefaults();

		objs.options = options;
		objs.sc = new SimController(options, runner);
		return objs;
}