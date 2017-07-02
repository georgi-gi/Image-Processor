#pragma once
#include "image.h"
#include "PixelBit.h"
class PBM :
	public Image
{
protected:
	PixelBit** bitMap;
private:
	virtual void fill(std::ifstream&) = 0;
	virtual void createNewImage(std::ofstream&, char*) = 0;
public:
	PBM(char type[], unsigned int width, unsigned int height, unsigned int endOfHeader, char* path);
	PBM(void);
	virtual ~PBM(void);
public:
	virtual void genGrayscale(std::ifstream&) = 0;
	virtual void genMonochrome(std::ifstream&) = 0;
	virtual void genRedHistogram(std::ifstream&) = 0;
	virtual void genGreenHistogram(std::ifstream&) = 0;
	virtual void genBlueHistogram(std::ifstream&) = 0;
};

