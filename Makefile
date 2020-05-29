#Makefile
CXXFLAGS = -Wall -std=c++11
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`

all: projet

noeud.o: noeud.cc error.h noeud.h tools.h constantes.h
	g++ $(CXXFLAGS) -o noeud.o -c noeud.cc

ville.o: ville.cc error.h ville.h noeud.h tools.h constantes.h
	g++ $(CXXFLAGS) -o ville.o -c ville.cc

tools.o: tools.cc tools.h constantes.h
	g++ $(CXXFLAGS) -o tools.o -c tools.cc

error.o: error.cc error.h constantes.h
	g++ $(CXXFLAGS) -o error.o -c error.cc

graphic.o: graphic.cc graphic.h noeud.h
	g++ $(CXXFLAGS) $(LINKING) -o graphic.o -c graphic.cc $(LINKING)

gui.o: gui.cc gui.h graphic.h
	g++ $(CXXFLAGS) $(LINKING) -o gui.o -c gui.cc $(LINKING)

projet.o: gui.h graphic.h projet.cc noeud.h
	g++ $(CXXFLAGS) $(LINKING) -o projet.o -c projet.cc $(LINKING)

projet: error.o tools.o noeud.o ville.o graphic.o gui.o projet.o
	g++ $(CXXFLAGS) $(LINKING) error.o tools.o noeud.o ville.o graphic.o gui.o projet.o -o projet $(LDLIBS)

clean:
	@echo "**** EFFACE MODULES ET EXECUTABLES ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ projet

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  g++ -MM -g $(CXXFLAGS) error.cc tools.cc noeud.cc ville.cc graphic.cc gui.cc projet.cc | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile
