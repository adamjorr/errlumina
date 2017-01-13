#ifndef __ERRLUMINA_MISMATCH_FINDER_INCLUDED__
#define __ERRLUMINA_MISMATCH_FINER_INCLUDED__

#include <memory>
#include <htslib/hts.h>
#include <htslib/sam.h>
#include <htslib/faidx.h>
#include <string>
#include "samio.h"

class MismatchFinder{
protected:
	SamReader* reader;
	faidx_t* faidx_p;
	char* ref;
	int tid;
public:
	MismatchFinder(SamReader* r, std::string ref_name);
	~MismatchFinder();
	void dump_mismatches(std::string fileout);
	void dump_mismatches();
};

static bool has_mismatch(bam1_t *b, char *ref, int ref_len);






#endif