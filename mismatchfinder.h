#ifndef __ERRLUMINA_MISMATCH_FINDER_INCLUDED__
#define __ERRLUMINA_MISMATCH_FINER_INCLUDED__

#include <memory>
#include <htslib/hts.h>
#include <htslib/sam.h>
#include <string>
#include "samio.h"

class MismatchFinder{
protected:
	SamReader reader;
public:
	MismatchFinder(SamReader r) : reader(r);
	void dump_mismatches(std::string fileout);
};

static bool has_mismatch(bam1_t *b, char *ref, int ref_len);






#endif