#include <QtGui>
#include <QApplication>
#include "LifeDrawingWidget.h"
#include <stdio.h>
#include <stdexcept>
#include <string>

int main( int argc, char **argv )
{
	QApplication app( argc, argv );
 
	try{
		LifeGUISimOptions *options = NULL;
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

		SimController sc(options, new LifeSimRunner());
		sc.simGenerations(options->getOutputGeneration());
		
		LifeDrawingWidget myWidget(options, &sc);
		myWidget.show();
	}catch (runtime_error e){
		fprintf(stderr, "Error encountered: %s\n", e.what());
	}

	return app.exec();
}
