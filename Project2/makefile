all: life_gui

clean:
	rm -f life life_gui life.o ArgumentParsing.o FileParsing.o Grid.o LifeView.o SimController.o SimOptions.o life_gui.o LifeDrawingWidget.o

tarball: proj2.tar.gz

proj2.tar.gz: makefile README 
	tar -czf proj2.tar.gz makefile README LifeHelp.txt

life_gui: 
	qmake-qt4 -project -o life_gui.pro
	qmake-qt4 -makefile -o gui_makefile life_gui.pro
	make -f gui_makefile

life: life.o ArgumentParsing.o FileParsing.o Grid.o LifeView.o SimController.o SimOptions.o
	g++ -o life life.o ArgumentParsing.o FileParsing.o Grid.o LifeView.o SimController.o SimOptions.o

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
