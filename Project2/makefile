all: life_gui life

clean:
	rm -f life life_gui life.o life_gui.o LifeDrawingWidget.o ArgumentParsing.o FileParsing.o Grid.o LifeView.o SimController.o SimOptions.o life_gui.o LifeDrawingWidget.o LifeScrollWidget.o LifeControlsWidget.o InitHelpers.o gui_makefile gui_makefile_fixed life_gui.pro moc_LifeControlsWidget.cpp moc_LifeDrawingWidget.cpp moc_LifeDrawingWidget.o moc_LifeScrollWidget.cpp moc_LifeScrollWidget.o moc_LifeControlsWidget.o

tarball: proj2.tar.gz

proj2.tar.gz: makefile README LifeHelp.txt LifeGuiHelp.txt ArgumentParsing.cpp ArgumentParsing.h FileParsing.cpp FileParsing.h Grid.cpp Grid.h life.cpp life_gui.cpp LifeDrawingWidget.cpp LifeDrawingWidget.h LifeView.cpp LifeView.h SimController.cpp SimController.h SimOptions.cpp SimOptions.h InitHelpers.cpp InitHelpers.h LifeControlsWidget.cpp LifeControlsWidget.h LifeScrollWidget.cpp LifeScrollWidget.h
	tar -czf proj2.tar.gz makefile README LifeHelp.txt LifeGuiHelp.txt ArgumentParsing.cpp ArgumentParsing.h FileParsing.cpp FileParsing.h Grid.cpp Grid.h life.cpp life_gui.cpp LifeDrawingWidget.cpp LifeDrawingWidget.h LifeView.cpp LifeView.h SimController.cpp SimController.h SimOptions.cpp SimOptions.h InitHelpers.cpp InitHelpers.h LifeControlsWidget.cpp LifeControlsWidget.h LifeScrollWidget.cpp LifeScrollWidget.h

#These dependencies are weird. I need it to know when to update, but I can't just say life_gui.o and LifeDrawingWidget.o
life_gui: life_gui.cpp LifeControlsWidget.h LifeControlsWidget.cpp LifeScrollWidget.h LifeScrollWidget.cpp  LifeDrawingWidget.h LifeDrawingWidget.cpp ArgumentParsing.o FileParsing.o Grid.o LifeView.o SimController.o SimOptions.o
	qmake-qt4 -project -o life_gui.pro
	qmake-qt4 -makefile -o gui_makefile life_gui.pro
	sed -e 's/life.o//g' <gui_makefile >gui_makefile_fixed
	make -f gui_makefile_fixed

life: life.o ArgumentParsing.o FileParsing.o Grid.o LifeView.o SimController.o SimOptions.o InitHelpers.o
	g++ -o life life.o ArgumentParsing.o FileParsing.o Grid.o LifeView.o SimController.o SimOptions.o InitHelpers.o

life.o: life.cpp ArgumentParsing.h FileParsing.h Grid.h LifeView.h SimController.h SimOptions.h
	g++ -c life.cpp

ArgumentParsing.o: ArgumentParsing.cpp ArgumentParsing.h SimOptions.h 
	g++ -c ArgumentParsing.cpp

FileParsing.o: FileParsing.cpp FileParsing.h SimOptions.h
	g++ -c FileParsing.cpp

Grid.o: Grid.cpp Grid.h SimOptions.h
	g++ -c Grid.cpp

LifeView.o: LifeView.cpp LifeView.h SimController.h SimOptions.h
	g++ -c LifeView.cpp

SimController.o: SimController.cpp SimController.h SimOptions.h Grid.h
	g++ -c SimController.cpp

SimOptions.o: SimOptions.cpp SimOptions.h 
	g++ -c SimOptions.cpp

InitHelpers.o: InitHelpers.cpp InitHelpers.h SimController.h ArgumentParsing.h FileParsing.h SimOptions.h
	g++ -c InitHelpers.cpp