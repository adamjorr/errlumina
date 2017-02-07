#ifndef __ERRLUMINA_REFTYPE_INCLUDED__
#define __ERRLUMINA_REFTYPE_INCLUDED__

class Reftype{
protected:
	//
public:
	Reftype(std::string reference_name);
	Reftype(faidx_t* faidx_p);
	~Reftype();
};



#endif