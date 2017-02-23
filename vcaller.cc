#include "vcaller.h"
#include <htslib/sam.h>
#include "samio.h"

//TODO: something to handle samples in BAM
void pileup_and_call(){
	int tid;
	int pos;
	int cov;
	bam_pileup1_t *pileup = nullptr;

	bam_plp_t iter = bam_plp_init(&plp_get_read, &reader);
	while( (pileup = bam_plp_auto(iter,&tid,&pos,&cov)) != nullptr){ //successfully pile up new position
		char refbase = ref.get_ref(reader.get_ref_name(tid))[pos];//get ref allele
		std::vector<char> alleles = get_piled_alleles(pileup, cov, refbase)//get array of alt + ref alleles
		//TODO:call variant(s) using array of alt alleles
		bcf1_t b = create_vcf_line()//TODO:create vcf entry
		writer.write_variant(b)//write vcf entry
	}
}

std::vector<char> call_variants(std::vector<char> alleles){
	int coverage = alleles.size();
	float threshold = .4;
	std::map<char, int> counts;
	for(auto it = alleles.begin(); it != alleles.end(); ++it){
		if (counts.count(it->first) == 0){
			it->second = 1;
		}
		else{
			it->second += 1;
		}
	}

	for(auto it = counts.begin(); it != counts.end(); ++it){
		if (it->second > threshhold){
			//it's a variant!
		}
	}


}

//TODO: cache this somewhere or we're gonna be slow
static long double binomial_cdf(int successes, int trials, long double p){
	long double cdf = 0.0;
	for(int i = 0; i < k; i++){
		cdf += binomial_pdf(i, trials, p);
	}
}

static long double binomial_pdf(int successes, int trials, long double p){
	return binomial_coeff(trials, successes) * (p ** successes) * (1 - p) ** (trials - successes);
}

//n choose k
static int binomial_coeff(int n, int k){
	if (k > n){
		throw std::runtime_error("error calculating binomial with n = " << n << " and k = " << k << std::endl);
	}
	int product = 1;
	for(int i=1; i <= k; i++){
		product *= ((n + 1 - i) / i);
	}
	return product;
}

static long double p_incorrect_call(int count, int coverage, long double error){
	long double probability = 0.0;
	for (int i = count; i <= coverage; ++i){
		probability += binomial_pdf(i, coverage, error)
	}
	return probability;
}

static int p_threshold(int coverage, long double error, long double cutoff_error){
	//returns the minimum count such that the probability of an erronious call is less than cutoff_error
	for (int i = 0; i <= coverage; i++){
		if (p_incorrect_call(i,coverage,error) < cutoff_error){
			return i;
		}
	}
	return 0;
}

int get_cutoff(int coverage){
	int maxindex = cutoffs.size() - 1;
	if (coverage > maxindex){
		cutoffs.reserve(coverage + 1);
		for(int i = maxindex; i <= coverage; ++i){
			cutoffs[i] = p_threshold(i, error, cutoff_error);
		}
	}
	return cutoffs[coverage];
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







