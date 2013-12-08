#include "LifeScrollWidget.h"

LifeScrollWidget::LifeScrollWidget(SimOptions *opts, SimController *ctrl){
	QScrollArea *scroll = new QScrollArea;
	setCentralWidget( scroll );
	LifeDrawingWidget *draw = new LifeDrawingWidget(opts, ctrl);
	draw->setGeometry(0,0,400,400);
	resize(300,300);
	scroll->setWidget(draw);
}

