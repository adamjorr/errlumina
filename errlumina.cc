#include "samio.h"
#include "mismatchfinder.h"
#include <string>

int main(){
	SamReader reader("testdata/test.sam");
	MismatchFinder mismatcher(reader,"testdata/test.fa");
	mismatcher.dump_mismatches("-");
}