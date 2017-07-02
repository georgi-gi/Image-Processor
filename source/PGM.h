#pragma once
#include "image.h"
#include "PixelGray.h"

///
/// Абстрактен клас PGM с базов клас Image, наследяван от класовете P2 и P5.
///	Класът съдържа указател към двумерен масив от обекти от тип PixelGray, който ще съхранява стойностите
///на всички пиксели в подаденото изображение.
/// Класът съдържа 2 нови чисто виртуални функции:
/// - fill(ifstream&) - прочита стойностите от подадения файл и ги записва в двумерния масив;
/// - createNewImage(ofstream&, char*) - създава ново изображение от записания в обекта двумерен масив
///bitMap.
/// Други функции:
/// - void turnMono() - функция с видимост protected, която преизчислява стойностите на пикселите и ги обръща
///монохромни
/// - bool isMono() - булева функция, която проверява дали подаденото изображение е вече монохромно. Връща 
///true, ако е монохромно и false - ако не е.
///
class PGM :
	public Image
{
protected:
	PixelGray** bitMap;
protected:
	void turnMono();
	bool isMono();
private:
	virtual void fill(std::ifstream&) = 0;
	virtual void createNewImage(std::ofstream&, char*) = 0;
public:
	PGM(char type[], unsigned int width, unsigned int height, unsigned int maxVal, unsigned int endOfHeader, char* path);
	PGM(void);
	PGM(const PGM&);
	PGM& operator= (const PGM&);
	virtual ~PGM(void);
public:
	virtual void genGrayscale(std::ifstream&) = 0;
	virtual void genMonochrome(std::ifstream&) = 0;
	virtual void genRedHistogram(std::ifstream&) = 0;
	virtual void genGreenHistogram(std::ifstream&) = 0;
	virtual void genBlueHistogram(std::ifstream&) = 0;
};

