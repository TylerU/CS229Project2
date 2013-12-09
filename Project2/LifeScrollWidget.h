#ifndef LIFE_SCROLL_H
#define LIFE_SCROLL_H

#include <QtGui>
#include <QWidget>
#include <QMainWindow>
#include "LifeDrawingWidget.h"

class LifeScrollWidget : public QMainWindow {
	Q_OBJECT
public:
	LifeScrollWidget(SimOptions *options, SimController *ctrl);
	virtual ~LifeScrollWidget(){}
	LifeDrawingWidget *displayWidget;
protected:
signals:

public slots:

};

#endif
