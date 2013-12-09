#include "LifeControlsWidget.h"
#include <QSpinBox>
#include <QBoxLayout>
#include <QLabel>

LifeControlsWidget::LifeControlsWidget(SimOptions *opts, SimController *ctrlr, LifeScrollWidget &scroll) : displayWidget(scroll)  {
	options = opts;
	controller = ctrlr;
	delay = 100;
	play = false;

	QLabel *gridLabel = new QLabel("Grid Size:");
	QLabel *delayLabel = new QLabel("Delay:");
	genLabel = new QLabel("Generation:\t 1");
	refreshGenLabel();

	QSpinBox *gridSizeSpin = new QSpinBox();
	gridSizeSpin->setMinimum(1);
	gridSizeSpin->setMaximum(1000);
	gridSizeSpin->setValue(opts->getBlockSize());

	QSpinBox *delaySpin = new QSpinBox();
	delaySpin->setMinimum(10);
	delaySpin->setMaximum(10000);
	delaySpin->setValue(delay);
	
	QPushButton *quitBtn = new QPushButton("Quit");
	QPushButton *restartBtn = new QPushButton("Restart");
	playBtn = new QPushButton("Play");
	QPushButton *stepBtn = new QPushButton("Step");
	
	QBoxLayout *box = new QBoxLayout(QBoxLayout::TopToBottom, this);
	
	box->addWidget(gridLabel);
	box->addWidget(gridSizeSpin);
	box->addWidget(delayLabel);
	box->addWidget(delaySpin);
	box->addWidget(genLabel);
	box->addWidget(quitBtn);
	box->addWidget(restartBtn);
	box->addWidget(playBtn);
	box->addWidget(stepBtn);
	box->addStretch(1);
	box->addSpacing(12);
	setLayout(box);

	connect(quitBtn, SIGNAL(clicked()), this, SLOT(onQuit()));
	connect(restartBtn, SIGNAL(clicked()), this, SLOT(onRestart()));
	connect(playBtn, SIGNAL(clicked()), this, SLOT(onPlay()));
	connect(stepBtn, SIGNAL(clicked()), this, SLOT(onStep()));
	connect(gridSizeSpin, SIGNAL(valueChanged(int)), this, SLOT(onSizeChange(int)));
	connect(delaySpin, SIGNAL(valueChanged(int)), this, SLOT(onDelayChange(int)));
}

void LifeControlsWidget::onQuit(){
	displayWidget.close();	
	close();
}

void LifeControlsWidget::onRestart(){
	controller->reset();	
	displayWidget.displayWidget->update();
	refreshGenLabel();
}

void LifeControlsWidget::onPlay(){
	if(!play){
		play = true;
		playBtn->setText("Pause");
		simAndDisplay();
	}
	else{
		play = false;
		playBtn->setText("Play");
	}
}

void LifeControlsWidget::simulateStep(){
	controller->simGenerations(1);
	displayWidget.displayWidget->update();
	refreshGenLabel();
}

void LifeControlsWidget::onStep(){
	if(play){
		play = false;
		playBtn->setText("Play");
	}
	else{
		simulateStep();
	}
}

void LifeControlsWidget::onDelayChange(int i){
	delay = i;
}

void LifeControlsWidget::onSizeChange(int i){
	options->setBlockSize(i);
	displayWidget.displayWidget->update();
}

void LifeControlsWidget::refreshGenLabel(){
	char buffer[200];
	sprintf(buffer, "Generation: %d", controller->getCurGeneration());

	genLabel->setText(buffer);
}

void LifeControlsWidget::simAndDisplay(){
	simulateStep();
	if(play){
		QTimer::singleShot(delay, this, SLOT(simAndDisplay()));
	}
}
