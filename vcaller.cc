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
		//get ref allele
		//get array of alt alleles
		//call variant(s) using array of alt alleles
		//create vcf entry
		//write vcf entry
	}
}

//typedef int (*bam_plp_auto_f)(void *data, bam1_t *b);
int plp_get_read(void *data, bam1_t *b){
	SamReader *reader = data;
	reader->next(b);
}







