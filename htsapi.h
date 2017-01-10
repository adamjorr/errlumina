#ifndef __ERRLUMINA_HTS_API_INCLUDED__
#define __ERRLUMINA_HTS_API_INCLUDED__

#include <memory>
#include "htslib/hts.h"
#include "htslib/sam.h"
#include <string>

class HtsAPI{
protected:
	std::unique_ptr<samFile> in;
	std::unique_ptr<bam_hdr_t> header;
	std::unique_ptr<hts_idx_t> idx;
	std::unique_ptr<hts_itr_t> iter;
	std::unique_ptr<std::string> region;
	bool region_exists;
	void open(std::string);
	void open(std::string, std::string);
public:
	bool has_region();
	int next(bam1_t *b);
	HtsAPI(std::string filename);
	HtsAPI(std::string filename, std::string region);
};

static bool has_mismatch(bam1_t *b, char *ref, int ref_len);








#endif