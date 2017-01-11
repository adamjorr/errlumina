#include "samio.h"

SamReader::SamReader(std::string filename){
	open(filename);
	this->region_exists = false;
}

SamReader::SamReader(std::string filename, std::string region){
	open(filename, region);
	this->region_exists = true;
}


//set in and header
void SamReader::open(std::string filename_in){
	samFile* htsin = sam_open(filename_in.c_str(), "r");
	if (htsin == nullptr) {
    	//error
        throw std::runtime_error("error");
    }
    this->in.reset(htsin);

    bam_hdr_t* htsheader = sam_hdr_read(htsin);
    if (htsheader == nullptr){
    	//error
    	throw std::runtime_error("error");
    }
    this->header.reset(htsheader);
}

//set in and header and idx and iter
void SamReader::open(std::string filename_in, std::string region){
	open(filename_in);
    hts_idx_t* idx = sam_index_load(in.get(), filename_in.c_str()); //&hts_idx_destroy
    if (idx != nullptr){
    	//error
    	throw std::runtime_error("error");
    }
    this->idx.reset(idx);

    hts_itr_t* iter = sam_itr_querys(idx, header.get(), region.c_str()); //&hts_itr_destroy
    if (iter != nullptr){
    	throw std::runtime_error("error");
    }
    this->iter.reset(iter);
}

//updates b with next read; returns >=0 on success
int SamReader::next(bam1_t* b){
	if (this->has_region()){
		return sam_itr_next(this->in.get(), this->iter.get(), b);
	}
	else{
		return sam_read1(this->in.get(), this->header.get(), b);
	}
}

bool SamReader::has_region(){
	return this->region_exists;
}

bam_hdr_t* SamReader::get_header(){
	return this->header.get();
}


//SamWriter class
void SamWriter::check_open_success(){
	if (this->outfh == 0) {
		throw std::runtime_error("error opening file");
	}
}

SamWriter::SamWriter(){
	this->outfh.reset(sam_open("-", "w"));
	this->header = nullptr;
	check_open_success();
}

SamWriter::SamWriter(std::string filename){
	this->outfh.reset(sam_open(filename.c_str(), "w"));
	this->header = nullptr;
	check_open_success();
}

SamWriter::SamWriter(bam_hdr_t* h){
	this->outfh.reset(sam_open("-", "w"));
	this->header.reset(h);
	check_open_success();
}

SamWriter::SamWriter(std::string filename, bam_hdr_t* h){
	this->outfh.reset(sam_open(filename.c_str(), "w"));
	this->header.reset(h);
	check_open_success();
}

void SamWriter::write_header(){
	if (this->header == nullptr){
		throw std::runtime_error("error writing header: null header");
	}
	else{
		if (sam_hdr_write(this->outfh.get(), this->header.get()) != 0){
			throw std::runtime_error("error writing header");
		}
	}
}

void SamWriter::write_read(const bam1_t* b){
	if (sam_write1(this->outfh.get(), this->header.get(), b) < 0){
		throw std::runtime_error("error writing read");
	}
}




//see https://github.com/samtools/samtools/blob/1bae2510c2f58e0332b84780b3c6bd438c58ed3c/bam_md.c#L58-L90
static bool has_mismatch(bam1_t *b, char *ref, int ref_len){
	uint8_t *seq = bam_get_seq(b);
	uint32_t *cigar = bam_get_cigar(b);
	bam1_core_t *c = &b->core;
	int i, x, y = 0;
	for (i = y = 0, x = c->pos; i < c->n_cigar; ++i){
		int j, l = cigar[i]>>4,  op = cigar[i]&0xf;
		if (op == BAM_CMATCH || op == BAM_CEQUAL || op == BAM_CDIFF){
			for (j = 0; j < l; ++j){
				int c1, c2, z = y + j;
				if (x + j >= ref_len || ref[x + j] == '\0') break; //out of bounds
				c1 = bam_seqi(seq, z);
				c2 = seq_nt16_table[(int)ref[x+j]];
				if (!((c1 == c2 && c1 != 15 && c2 != 15) || c1 == 0)){ //doesn't match
					return true;
				}
			}
			if (j < l) break; //break again if we have to break because we're out of bounds
			x += l;
			y += l;
		}
		else{
			return true;
		}
	}
}





