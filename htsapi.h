#ifndef __ERRLUMINA_HTS_API_INCLUDED__
#define __ERRLUMINA_HTS_API_INCLUDED__

#include <memory>
#include <htslib/hts.h>
#include <htslib/sam.h>
#include <string>

struct samFileDeleter{
	samFileDeleter() {};
	void operator()(samFile* p) const {
		sam_close(p);
	};
};

struct samIdxDeleter{
	samIdxDeleter() {};
	void operator()(hts_idx_t* p) const {
		hts_idx_destroy(p);
	};
};

struct samItrDeleter{
	samItrDeleter() {};
	void operator()(hts_itr_t* p) const {
		hts_itr_destroy(p);
	};
};

struct bamHdrDeleter{
	bamHdrDeleter() {};
	void operator()(bam_hdr_t* p) const {
		bam_hdr_destroy(p);
	};
};

class HtsAPI{
protected:
	std::unique_ptr<samFile,samFileDeleter> in;
	std::unique_ptr<bam_hdr_t,bamHdrDeleter> header;
	std::unique_ptr<hts_idx_t,samIdxDeleter> idx;
	std::unique_ptr<hts_itr_t,samItrDeleter> iter;
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