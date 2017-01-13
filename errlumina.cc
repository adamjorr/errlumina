#include "samio.h"
#include "mismatchfinder.h"
#include <string>

int main(){
	SamReader reader;
	MismatchFinder mismatcher(reader,"foo");
	mismatcher.dump_mismatches("-");
}