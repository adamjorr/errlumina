errlumina : errlumina.o samio.o mismatchfinder.o
	g++ --std=c++11 -lhts errlumina.o samio.o -o errlumina

mismatchfinder.o : mismatchfinder.h mismatchfinder.cc
	g++ --std=c++11 -lhts -c mismatchfinder.cc

samio.o : samio.h samio.cc
	g++ --std=c++11 -lhts -c samio.cc

errlumina.o : errlumina.cc
	g++ --std=c++11 -c errlumina.cc

