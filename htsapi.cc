//TODO: be sure all pointers have the correct destructors from htslib
//TODO: implement next

HtsAPI(std::string filename){
	open(filename);
	this->region_exists = false;
}

HtsAPI(std::string filename, std::string region){
	open(filename, region);
	this->region_exists = true;
}


//set in and header
static void HtsAPI::open(std::string filename_in){
	if ((in = sam_open(filename_in.c_str(), "r")) == 0) {
    	//error
        throw std::runtime_error("error");
    }
    this->in = std::uniq_ptr(in);

    if ((header = sam_hdr_read(in)) == 0){
    	//error
    	throw std::runtime_error("error");
    }
    this->header = std::uniq_ptr(header);
}

//set in and header and idx and iter
static void HtsAPI::open(std::string filename_in, std::string region){
	prep_open(filename_in);
    std::uniq_ptr<hts_idx_t> idx(sam_index_load(in, filename_in.c_str()),&hts_idx_destroy)
    if (idx != nullptr){
    	//error
    	throw std::runtime_error("error");
    }
    this->idx = idx;

    std::uniq_ptr<hts_itr_t> iter(sam_itr_querys(idx.pointer, header, region.c_str()), &hts_itr_destroy);
    if (iter != nullptr){
    	throw std::runtime_error("error");
    }
    this->iter = iter;
}

int HtsAPI::next(bam1_t* b){
	
}

bool HtsAPI::has_region(){
	return this->region_exists;
}

//see https://github.com/samtools/samtools/blob/9df632/bam_md.c#L58-L90
static bool HtsAPI::has_mismatch(bam1_t *b, char *ref, int ref_len){
	uint8_t *seq = bam_get_seq(b);
	uint32_t *cigar = bam_get_cigar(b);
	bam1_core_t *c = &b->core;
	int i, x, y = 0;
	for (i = y = 0, x = c->post; i < c->n_cigar; ++i){
		int j, l = cigar[i]>>4,  op = cigar[i]&0xf;
		if (op == BAM_CMATCH || op == BAM_CEQUAL || op == BAM_CDIFF){
			for (j = 0; j < l; ++j){
				int c1, c2, z = y + j;
				if (x + j >= ref_len || ref[x + j]] == '\0') break; //out of bounds
				c1 = bam_seqi(seq, z);
				c2 = seq_nt16_table[(int)ref[x+j]];
				if (!((c1 == c2 && c1 != 15 && c2 != 15) || c1 == 0)){ //doesn't match
					return true
				}
			}
			if (j < l) break //break again if we have to break because we're out of bounds
			x += l;
			y += l;
		}
		else{
			return true
		}
	}
}





