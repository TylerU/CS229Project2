#include <QtGui>
#include <QApplication>
#include <QString>

#include <stdio.h>
#include <stdexcept>
#include <string>
#include "SimOptions.h"
#include "FileParsing.h"
#include "ArgumentParsing.h"
#include "Grid.h"
#include "SimController.h"
#include "LifeView.h"
#include "LifeScrollWidget.h"
#include "InitHelpers.h"

int main( int argc, char **argv )
{
	QApplication app( argc, argv );
 
	
	LifeObjects objs;
	try{
		objs = initLife(argc, argv);
		objs.sc->simGenerations(objs.options->getOutputGeneration());
	}catch (runtime_error e){
		fprintf(stderr, "Error encountered: %s\n", e.what());
		return 1;
	}
	
	if(objs.options->getShowHelp()){
		append_file_to_file("LifeGuiHelp.txt", stdout);
		return 0;
	}
	else{
		LifeScrollWidget myWidget(objs.options, objs.sc);
		myWidget.setWindowTitle(QString(objs.options->name.getString().c_str()));
		myWidget.show();
		
		return app.exec();
	}
}
