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
		fai_destroy(p);
	};
};

struct FaiDeleter{
	FaiDeleter() {};
	void operator()(char* p) const {
		free(p);
	}
};


class MismatchFinder{
protected:
	SamReader reader;
	std::unique_ptr<faidx_t, FaidxDeleter> faidx_p;
	std::unique_ptr<char, FaiDeleter> ref;
	int tid;
public:
	MismatchFinder(SamReader r, std::string ref_name);
	void dump_mismatches(std::string fileout);
};

static bool has_mismatch(bam1_t *b, char *ref, int ref_len);






#endif