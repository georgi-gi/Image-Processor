#pragma once
#include "ppm.h"

///
/// Клас P6, наследник на PPM. Наследява от родителите информацията за изображението (тип, височина,
///ширина, максимална стойност), двумерния масив, съдържащ стойностите на цветовете в пикселите, както
///и функциите isGrayscale(), isMonochrome() и turnGrayscale().
///
class P6 :
	public PPM
{
private:
	virtual void fill(std::ifstream&);
	virtual void createNewImage(std::ofstream&, char*);
public:
	P6();
	P6(char type[], unsigned int, unsigned int, unsigned int, unsigned int, char*);
	P6(const P6&);
	P6& operator=(const P6&);
	virtual ~P6(void);
public:
	virtual void genGrayscale(std::ifstream&);
	virtual void genMonochrome(std::ifstream&);
	virtual void genRedHistogram(std::ifstream&);
	virtual void genGreenHistogram(std::ifstream&);
	virtual void genBlueHistogram(std::ifstream&);
};

