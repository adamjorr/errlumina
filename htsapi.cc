


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
				if ((c1 == c2 && c1 != 15 && c2 != 15) || c1 == 0){ //a match
					return true
				}
				else{
					return false
				}
			}
			if (j < l) break
			x += 1;
			y += 1;
		}
		else if (op == BAM_CDEL){
			return true
		}
		else if (op == BAM_CINS || op == BAM_CSOFT_CLIP){
			return true
		}
		else if (op == BAM_CREFSKIP){
			return true
		}
	}
}

static bool HtsAPI::has_mismatch(bam1_t *b){
	uint32_t *cigar = bam_get_cigar(b);
	bam1_core_t *c = &b->core;
	for (i = 0; i < c->n_cigar; ++i){
		op = (cigar[i]>>4, cigar[i]&0xf);
		if (op != BAM_CMATCH || op != BAM_CEQUAL){
			return true
		}
	}
	return false
}

