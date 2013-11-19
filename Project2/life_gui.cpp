#include <QtGui>
#include <QApplication>

#include <stdio.h>
#include <stdexcept>
#include <string>
#include "SimOptions.h"
#include "FileParsing.h"
#include "ArgumentParsing.h"
#include "Grid.h"
#include "SimController.h"
#include "LifeView.h"
#include "LifeDrawingWidget.h"
int main( int argc, char **argv )
{
	QApplication app( argc, argv );
 
	
	LifeGUISimOptions *options = NULL;
	SimController *controller = NULL;
	try{
		LifeGUIArgumentParser ap(argc, argv);
		FileParser fp(ap.getFileNameOrEmptyString());
		string type = fp.getId();
		if(type == "Life"){
			fp.setSimOptions(options = new LifeGUISimOptions());
			ap.setOptions(options);
			fp.setParser(new ParserCreator(options));
			options->setSimType(type);
		}
		fp.readFile();
		ap.parse();
		options->resolveDefaults();

		controller = SimController(options, new LifeSimRunner());
		sc->simGenerations(options->getOutputGeneration());
	}catch (runtime_error e){
		fprintf(stderr, "Error encountered: %s\n", e.what());
	}
	
	LifeDrawingWidget myWidget(options, &sc);

	myWidget.show();

	return app.exec();
}
