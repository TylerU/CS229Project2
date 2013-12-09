#ifndef LIFE_CONTROLS_H
#define LIFE_CONTROLS_H

#include <QtGui>
#include <QWidget>
#include <QLabel>
#include <QDialog>
#include <QTimer>
#include "SimController.h"
#include "LifeScrollWidget.h"

class LifeControlsWidget : public QWidget {
	Q_OBJECT
public:
	LifeControlsWidget(SimOptions *options, SimController *ctrl, LifeScrollWidget &scrollWidget);
	virtual ~LifeControlsWidget(){}
	virtual QSize sizeHint() const { return QSize(400,400); }
protected:
	LifeScrollWidget &displayWidget;
	SimOptions *options;
	SimController *controller;
	QLabel *genLabel;
	QPushButton *playBtn;
	void refreshGenLabel();
	void simulateStep();
	int delay;
	bool play;
signals:

public slots:
	void onQuit();
	void onRestart();
	void onPlay();
	void onStep();
	void onDelayChange(int i);
	void onSizeChange(int i);
	void simAndDisplay();
};

#endif
