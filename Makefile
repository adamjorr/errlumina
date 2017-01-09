errlumina : errlumina.o htsapi.o
	g++ --std=c++11 errlumina.o htsapi.o -lhts -o errlumina

htsapi.o : htsapi.h htsapi.cc
	g++ --std=c++11 -c htsapi.cc

errlumina.o : errlumina.cc
	g++ --std=c++11 -c errlumina.cc
