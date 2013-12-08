#include "LifeDrawingWidget.h"

 
/*LifeDrawingWidget::LifeDrawingWidget(LifeGUISimOptions *options, SimController *ctrl){
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
			int width  = (numVerticalLines) * opts->getBlockSize();
			int height = (numHorizontalLines) * opts->getBlockSize();
			int lineWidth = 1;
				
			resize(width, height);			
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
			int normX = x;
			int normY = y;
			normY -= opts->windowY.getLow();
			normX -= opts->windowX.getLow();
			int lineSizeOffset = 1;
			if(!opts->drawLines()){
				lineSizeOffset = 0;
			}	
			QRect rect(normX * opts->getBlockSize() + lineSizeOffset, normY * opts->getBlockSize() + lineSizeOffset, opts->getBlockSize() - lineSizeOffset, opts->getBlockSize() - lineSizeOffset); 
			QColor useColor = QColor::fromRgb(color.getFirst(), color.getSecond(), color.getThird());
			painter.fillRect(rect, useColor);
		}
	}

}
*/