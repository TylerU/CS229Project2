#include <QtGui>
#include <QApplication>
#include "LifeDrawingWidget.h"
 
int main( int argc, char **argv )
{
  QApplication app( argc, argv );
 
  MyWidget myWidget;
  myWidget.show();
 
  return app.exec();
}
