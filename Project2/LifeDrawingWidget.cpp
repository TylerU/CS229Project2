#include "LifeDrawingWidget.h"

 
LifeDrawingWidget::LifeDrawingWidget(LifeGUISimOptions *options, SimController *ctrl){
	if(options == NULL || ctrl == NULL) throw new runtime_error("Cannot have null options for simulation controller");
	opts = options;
	controller = ctrl;
}

void LifeDrawingWidget::paintEvent(QPaintEvent *event)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
 
		QPainter painter(this);

		if(opts->drawLines()){
			int numVerticalLines = opts->windowX.getHigh() - opts->windowX.getLow() + 1;
			int numHorizontalLines = opts->windowY.getHigh() - opts->windowY.getLow() + 1;
			int width  = (numVerticalLines-1) * opts->getBlockSize();
			int height = (numHorizontalLines-1) * opts->getBlockSize();
			int lineWidth = 1;
			
			QPen myPen(Qt::black, lineWidth, Qt::SolidLine);
			painter.setPen(myPen);

			for (int i = 0; i < numVerticalLines; ++i)
				painter.drawLine(i * opts->getBlockSize(), 0, i * opts->getBlockSize(), height);

			for (int j = 0; j < numHorizontalLines; ++j)
				painter.drawLine(0, j * opts->getBlockSize(), width, j*opts->getBlockSize());
		}

	for(int y = opts->windowY.getLow(); y <= opts->windowY.getHigh(); y++){
		for(int x = opts->windowX.getLow(); x <= opts->windowX.getHigh(); x++){
			Triple color = opts->getColorForState(controller->getStateOfCoord(x,y));
			QRect rect(x * opts->getBlockSize(), y * opts->getBlockSize(), opts->getBlockSize(), opts->getBlockSize()); 
			QColor color = QColor::fromRgba(color.getFirst(), color.getSecond(), color.getThird(), 255);
			painter.fillRect(rect, color);
		}
	}

}
