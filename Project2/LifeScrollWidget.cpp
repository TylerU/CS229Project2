#include "LifeScrollWidget.h"

LifeScrollWidget::LifeScrollWidget(SimOptions *opts, SimController *ctrl){
	QScrollArea *scroll = new QScrollArea;
	setCentralWidget( scroll );
	displayWidget = new LifeDrawingWidget(opts, ctrl);
	displayWidget->setGeometry(0,0,100,100);
	resize(400,400);
	scroll->setWidget(displayWidget);	
}

