#ifndef __ERRLUMINA_MISMATCH_FINDER_INCLUDED__
#define __ERRLUMINA_MISMATCH_FINER_INCLUDED__

#include <memory>
#include <htslib/hts.h>
#include <htslib/sam.h>
#include <htslib/faidx.h>
#include <string>
#include "samio.h"

struct FaidxDeleter{
	FaidxDeleter() {};
	void operator()(faidx_t* p) const {
		free(p);
	};
};


class MismatchFinder{
protected:
	SamReader reader;
	std::unique_ptr<faidx_t, FaidxDeleter> faidx_p;
public:
	MismatchFinder(SamReader r, std::string ref_name) : reader(r);
	void dump_mismatches(std::string fileout);
};

static bool has_mismatch(bam1_t *b, char *ref, int ref_len);






#endif