#pragma once
#include "ppm.h"

///
/// Клас P3, наследник на PPM. Наследява от родителите информацията за изображението (тип, височина,
///ширина, максимална стойност), двумерния масив, съдържащ стойностите на цветовете в пикселите, както
///и функциите isGrayscale(), isMonochrome() и turnGrayscale().
///
class P3 :
	public PPM
{
private:
	virtual void fill(std::ifstream&);
	virtual void createNewImage(std::ofstream&, char*);
public:
	P3();
	P3(char type[], unsigned int, unsigned int, unsigned int, unsigned int, char*);
	P3(const P3&);
	P3& operator=(const P3&);
	virtual ~P3(void);
public:
	virtual void genGrayscale(std::ifstream&);
	virtual void genMonochrome(std::ifstream&);
	virtual void genRedHistogram(std::ifstream&);
	virtual void genGreenHistogram(std::ifstream&);
	virtual void genBlueHistogram(std::ifstream&);
};

