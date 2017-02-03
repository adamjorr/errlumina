#ifndef __ERRLUMINA_MISMATCH_FINDER_INCLUDED__
#define __ERRLUMINA_MISMATCH_FINDER_INCLUDED__

#include <memory>
#include <htslib/hts.h>
#include <htslib/sam.h>
#include <htslib/faidx.h>
#include <string>
#include <iostream>
#include <ostream>
#include <fstream>
#include "samio.h"

class MismatchFinder{
protected:
	SamReader* reader;
	faidx_t* faidx_p;
	char* ref;
	int ref_len;
	int tid;
public:
	MismatchFinder(SamReader* r, std::string ref_name);
	~MismatchFinder();
	void dump_mismatches(std::string fileout);
	void dump_mismatches();
	void dump_locations(std::string fileout);
	void dump_locations();
	void dump_locations(std::ostream*);
	void check_tid(bam1_t*);
};

static bool has_mismatch(bam1_t *b, char *ref, int ref_len);
static int mismatch_location(bam1_t *b, char *ref, int ref_len);
static std::string get_sequence(bam1_t *b);
static std::string get_cigar_str(bam1_t *b);

#endif