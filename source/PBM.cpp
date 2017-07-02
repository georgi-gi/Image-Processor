#include "PBM.h"

PBM::PBM(char type[], unsigned int width, unsigned int height, unsigned int endOfHeader, char* path) : Image()
{
	bitMap = NULL;

	this->filePath = new char[strlen(path) + 1];
	strcpy(this->filePath, path);

	for (int i = 0; i < 3; ++i)
		this->type[i] = type[i];
	this->width = width;
	this->height = height;
	this->maxVal = 1;
	this->endOfHeader = endOfHeader;

	try
	{
		bitMap = new PixelBit*[height];
		for (int i = 0; i < height; ++i)
		{
			bitMap[i] = new PixelBit[width];
		}
	}
	catch(std::bad_alloc&)
	{
		std::cerr << "Couldn't allocate memory.\n";
	}
}

PBM::PBM(void)
{
}


PBM::~PBM(void)
{
	for (int i = 0; i < height; ++i)
	{
		delete[] bitMap[i];
	}
	delete[] bitMap;
}