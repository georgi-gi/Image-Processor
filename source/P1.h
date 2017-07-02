#pragma once
#include "pbm.h"

///
///
///
class P1 :
	public PBM
{
private:
	virtual void fill(std::ifstream&);
	virtual void createNewImage(std::ofstream&, char*);
public:
	P1(void);
	P1(char type[], unsigned int, unsigned int, unsigned int, char*);
	virtual ~P1(void);
public:
	virtual void genGrayscale(std::ifstream&);
	virtual void genMonochrome(std::ifstream&);
	virtual void genRedHistogram(std::ifstream&);
	virtual void genGreenHistogram(std::ifstream&);
	virtual void genBlueHistogram(std::ifstream&);
};

