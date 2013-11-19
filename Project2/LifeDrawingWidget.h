#ifndef MYWIDGET_H
#define MYWIDGET_H
 
#include <QtGui>
#include <QWidget>
 
class LifeDrawingWidget : public QWidget{
    Q_OBJECT
public:
    LifeDrawingWidget(){}
    virtual ~LifeDrawingWidget(){} 
protected:
    void paintEvent(QPaintEvent *event);
signals:
 
public slots:
 
};
 
#endif // MYWIDGET_H
