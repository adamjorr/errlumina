#include "mismatchfinder.h"

MismatchFinder::MismatchFinder(SamReader* r, std::string ref_name) : tid(-2) {
	reader = r;
	faidx_t* faidx = fai_load(ref_name.c_str());
	if (faidx == nullptr){
		throw std::runtime_error("error");
	}
	faidx_p = faidx;
}

MismatchFinder::~MismatchFinder(){
	if (faidx_p != nullptr){
		fai_destroy(faidx_p);
	}
	if (ref != nullptr){
		free(ref);
	}
}

// int len;
    // ref_file = argc > optind + 1 ? argv[optind+1] : ga.reference;
    // fai = fai_load(ref_file);
//     ref = fai_fetch(fai, header->target_name[b->core.tid], &len);

//to get ref and len:
//https://github.com/samtools/htslib/blob/develop/htslib/faidx.h
void MismatchFinder::dump_mismatches(std::string fileout){
	SamWriter w(this->reader->get_header());
	w.write_header();
	bam1_t *b = NULL;
	b = bam_init1();
	int ref_len;
	int r;

	while((r = this->reader->next(b)) >= 0){
		if (tid != b->core.tid){
			ref = fai_fetch(faidx_p,this->reader->get_header()->target_name[b->core.tid],&ref_len);
			if (ref == nullptr){
				throw std::runtime_error("error getting ref");
			}
		}
		if (has_mismatch(b, ref, ref_len)){
			w.write_read(b);
		}
	}
}

void MismatchFinder::dump_mismatches(){
	dump_mismatches("-");
}

void MismatchFinder::dump_locations(std::ostream *os){
	bam1_t *b = NULL;
	b = bam_init1();
	int ref_len;
	int r;
	int location;

	while((r = this->reader->next(b)) >= 0){
		if (tid != b->core.tid){
			ref = fai_fetch(faidx_p,this->reader->get_header()->target_name[b->core.tid],&ref_len);
			if (ref == nullptr){
				throw std::runtime_error("error getting ref");
			}
		}
		location = mismatch_location(b,ref,ref_len);
		if (location >= 0){
			*os << location << std::endl;
		}
	}
}

void MismatchFinder::dump_locations(std::string fileout){
	if (fileout == "-"){
		dump_locations();
	}
	else{
		std::ofstream ofs(fileout, std::ofstream::out);
		dump_locations(&ofs);
		ofs.close();
	}
}

void MismatchFinder::dump_locations(){
	MismatchFinder::dump_locations(&std::cout);
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
	return false;
}

//see https://github.com/samtools/samtools/blob/1bae2510c2f58e0332b84780b3c6bd438c58ed3c/bam_md.c#L58-L90
static int mismatch_location(bam1_t *b, char *ref, int ref_len){
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
					return z;
				}
			}
			if (j < l) break; //break again if we have to break because we're out of bounds
			x += l;
			y += l;
		}
		else{
			return y;
		}
	}
	return -1;
}

