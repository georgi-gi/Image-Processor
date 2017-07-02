#pragma once
#include "pbm.h"
class P4 :
	public PBM
{
private:
	virtual void fill(std::ifstream&);
	virtual void createNewImage(std::ofstream&, char*);
public:
	P4(void);
	P4(char type[], unsigned int, unsigned int, unsigned int, char*);
	P4(const P4&);
	P4& operator= (const P4&);
	~P4(void);
public:
	virtual void genGrayscale(std::ifstream&);
	virtual void genMonochrome(std::ifstream&);
	virtual void genRedHistogram(std::ifstream&);
	virtual void genGreenHistogram(std::ifstream&);
	virtual void genBlueHistogram(std::ifstream&);
};

