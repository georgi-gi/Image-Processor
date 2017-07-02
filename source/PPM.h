#pragma once
#include "image.h"
#include "PixelColour.h"

///
/// Абстрактен клас PGM с базов клас Image, наследяван от класовете P3 и P6.
///	Класът съдържа указател към двумерен масив от обекти от тип PixelColour, който ще съхранява стойностите
///на всички пиксели в подаденото изображение.
/// Класът съдържа 2 нови чисто виртуални функции:
/// - fill(ifstream&) - прочита стойностите от подадения файл и ги записва в двумерния масив;
/// - createNewImage(ofstream&, char*) - създава ново изображение от записания в обекта двумерен масив
///bitMap.
/// Други функции:
/// - void turnGray() - функция с видимост protected, която преизчислява стойностите на пикселите и ги обръща
///черно-бели (grayscale)
/// - bool isMonochrome() - булева функция, която проверява дали подаденото изображение е вече монохромно. Връща 
///true, ако е монохромно и false - ако не е.
/// - bool isGrayscale() - булева функция, която проверява дали подаденото изображение е вече черно-бяло
///(grayscale). Връща true, ако е, и false - ако не е.
///

class PPM :
	public Image
{
protected:
	PixelColour** bitMap;
protected:
	void turnGray();
	void turnMono();
	bool isGrayscale();
	bool isMonochrome();
private:
	virtual void fill(std::ifstream&) = 0;
	virtual void createNewImage(std::ofstream&, char*) = 0;
public:
	PPM();
	PPM(const PPM&);
	PPM& operator= (const PPM&);
	PPM(char type[], unsigned int width, unsigned int height, unsigned int maxVal, unsigned int endOfHeader, char* path);
	virtual ~PPM();
public:
	virtual void genGrayscale(std::ifstream&) = 0;
	virtual void genMonochrome(std::ifstream&) = 0;
	virtual void genRedHistogram(std::ifstream&) = 0;
	virtual void genGreenHistogram(std::ifstream&) = 0;
	virtual void genBlueHistogram(std::ifstream&) = 0;
};

