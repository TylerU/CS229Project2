#ifndef MYWIDGET_H
#define MYWIDGET_H
 
#include <QtGui>
#include <QWidget>
#include "SimOptions.h"
#include "SimController.h"
#include <stdexcept>
class LifeDrawingWidget : public QWidget{
    Q_OBJECT
public:
    LifeDrawingWidget(SimOptions *options, SimController *ctrl);
    virtual ~LifeDrawingWidget(){} 
protected:
	SimOptions *opts; 
	SimController *controller;
    void paintEvent(QPaintEvent *event);
signals:
 
public slots:
 
};
 
#endif // MYWIDGET_H
