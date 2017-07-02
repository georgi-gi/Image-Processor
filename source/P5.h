#pragma once
#include "pgm.h"

///
/// Клас P5, наследник на PGM. Наследява от родителите информацията за изображението (тип, височина,
///ширина, максимална стойност), двумерния масив, съдържащ стойностите на цветовете в пикселите, както
///и функциите isMono() и turnMono().
///
class P5 :
	public PGM
{
private:
	virtual void fill(std::ifstream&);
	virtual void createNewImage(std::ofstream&, char*);
public:
	P5(void);
	P5(char type[], unsigned int, unsigned int, unsigned int, unsigned int, char*);
	P5(const P5&);
	P5& operator= (const P5&);
	virtual ~P5(void);
public:
	virtual void genGrayscale(std::ifstream&);
	virtual void genMonochrome(std::ifstream&);
	virtual void genRedHistogram(std::ifstream&);
	virtual void genGreenHistogram(std::ifstream&);
	virtual void genBlueHistogram(std::ifstream&);
};

