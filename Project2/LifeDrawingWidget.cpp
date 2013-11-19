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

		/*for (int i = 0; i < image.width(); ++i) {
			for (int j = 0; j < image.height(); ++j) {
				QRect rect = pixelRect(i, j);
				if (!event->region().intersect(rect).isEmpty()) {
					QColor color = QColor::fromRgba(image.pixel(i, j));
					painter.fillRect(rect, color);
				}
			}
		}*/
}
