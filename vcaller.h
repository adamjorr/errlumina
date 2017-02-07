#ifndef __ERRLUMINA_VCALLER_INCLUDED__
#define __ERRLUMINA_VCALLER_INCLUDED__

class VCaller{
protected:
	SamReader reader;
	faidx_t* faidx_p;
	VCFwriter writer;
public:
	VCaller(std::string samfile, std::string reffile, std::string outfile);
	void call_variants();
};









#endif