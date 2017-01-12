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

class SamReader{
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
	bam_hdr_t* get_header();
	int next(bam1_t *b);
	SamReader();
	SamReader(const std::string filename);
	SamReader(const std::string filename, const std::string region);
	SamReader(const SamReader &r);
};

class SamWriter{
protected:
	std::unique_ptr<samFile,samFileDeleter> outfh;
	std::unique_ptr<bam_hdr_t,bamHdrDeleter> header;
public:
	SamWriter(std::string, bam_hdr_t*);
	SamWriter(std::string);
	SamWriter(bam_hdr_t*);
	SamWriter();
	void check_open_success(); //void, throws error if not open
	void write_header(); //throws error if header is null or fails to write
	void write_read(const bam1_t*);
};

#endif