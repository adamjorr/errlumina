#ifndef __ERRLUMINA_HTS_API_INCLUDED__
#define __ERRLUMINA_HTS_API_INCLUDED__

#include <memory>
#include <hts>

class HtsAPI{
protected:
	std::uniq_ptr<sam_fh_t> in;
	std::uniq_ptr<sam_hdr_t> header;
	std::uniq_ptr<sam_idx_t> idx;
	std::uniq_ptr<sam_itr_t> iter;
	std::uniq_ptr<std::string> region;
	bool region_exists;
	void open(std::string);
	void open(std::string, std::string);
public:
	bool has_region();
	int next(bam1_t *b);
	HtsAPI(std::string filename);
	HtsAPI(std::string filename, std::string region);
	static bool has_mismatch(bam1_t *b, char *ref, int ref_len);
};









#endif