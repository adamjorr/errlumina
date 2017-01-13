#include "samio.h"
#include "mismatchfinder.h"
#include <string>
#include <iostream>

int main(int argc, char *argv[]){
	std::clog << "Starting main()\n";
	SamReader reader("testdata/test.sam");
	std::clog << "Reader initialized successfully\n";
	MismatchFinder mismatcher(&reader,"testdata/test.fa");
	std::clog << "Mismatcher initialized successfully\n";
	mismatcher.dump_mismatches();
	std::clog << "Finished dumping. Exiting. . .\n";
}