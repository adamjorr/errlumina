#include "vcaller.h"
#include <htslib/sam.h>
#include "samio.h"

//TODO: something to handle samples in BAM
void call_variants(){
	int tid;
	int pos;
	int cov;
	bam_pileup1_t *pileup = nullptr;

	bam_plp_t iter = bam_plp_init(&plp_get_read, &reader);
	while( (pileup = bam_plp_auto(iter,&tid,&pos,&cov)) != nullptr){ //successfully pile up new position
		std::string ref = //TODO:get ref allele
		std::vector<char> alleles = get_piled_alleles(pileup, cov, ref)//TODO:get array of alt + ref alleles
		//TODO:call variant(s) using array of alt alleles
		bcf1_t b = create_vcf_line()//TODO:create vcf entry
		writer.write_variant(b)//write vcf entry
	}
}

//possible optimization: store sequence strings in a hash w/ alignment, throw out of hash once no longer in pileup
std::vector<char> get_piled_alleles(bam_pileup1_t* pileup, int cov, char ref){
	std::vector<char> alleles();
	alleles.reserve(cov + 1);
	alleles[0] = ref;
	for (i = 0; i < cov; i++){
		bam1_t* alignment = pileup[i].b;
		uint8_t *seq = bam_get_seq(alignment);
		int pos = pileup[i].qpos;
		int baseint = bam_seqi(seq,pos)
		alleles[i + 1] = seq_nt16_str[baseint];
	}
	return alleles;
}

bcf1_t create_vcf_line(bcf_hdr_t *hdr, std::vector<char> alleles){

}

//typedef int (*bam_plp_auto_f)(void *data, bam1_t *b);
int plp_get_read(void *data, bam1_t *b){
	SamReader *reader = data;
	reader->next(b);
}







