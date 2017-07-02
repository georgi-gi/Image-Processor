#pragma once
#include "pgm.h"

///
/// Клас P2, наследник на PGM. Наследява от родителите информацията за изображението (тип, височина,
///ширина, максимална стойност), двумерния масив, съдържащ стойностите на цветовете в пикселите, както
///и функциите isMono() и turnMono().
///
class P2 :
	public PGM
{
private:
	virtual void fill(std::ifstream&);
	virtual void createNewImage(std::ofstream&, char*);
public:
	P2(void);
	P2(char type[], unsigned int, unsigned int, unsigned int, unsigned int, char*);
	P2(const P2&);
	P2& operator= (const P2&);
	virtual ~P2(void);
public:
	virtual void genGrayscale(std::ifstream&);
	virtual void genMonochrome(std::ifstream&);
	virtual void genRedHistogram(std::ifstream&);
	virtual void genGreenHistogram(std::ifstream&);
	virtual void genBlueHistogram(std::ifstream&);
};

