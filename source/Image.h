#pragma once
#include <fstream>
#include <iostream>

///	Абстрактен клас Image, базов за PPM, PGM и PBM. 
///	Съдържа следните променливи с protected достъпност:
/// - char* filePath - съхранява пълния път на подадения при входа през конзолата файл
/// - char type[3] - съхранява типа на изображението (третата клетка е за терминиращата нула)
/// - unsigned int width - ширина на изображението (брой пиксели)
/// - unsigned int height - височина на изображението (брой пиксели)
/// - unsigned int maxVal - най-голяма стойност в изображението
///
///	Функции:
/// - void createNewPath(char*, char*, char*, char*) - функция, която генерира името на новия файл, без да
///променя директорията, от която е подаден на програмата. Приема като параметри 4 char-масива: директория
///с досегашното име; такъв, в който ще се запише новия пълен път със смененото име; разширението на подадения
///файл; име на функцията, която се изпълнява върху изображението (grayscale, monochrome, red, green or
///blue histogram.
/// - 5 чисто виртуални функции (genGrayscale(), genMonochrome(), genRedHistogram(), genBlueHistogram(),
///genGreenHistogram()) всяка от които приема референция към потока от изображението.
///
class Image
{
protected:
	char* filePath;
	char type[3];
	unsigned int width;
	unsigned int height;
	unsigned int maxVal;
	unsigned int endOfHeader;
	void createNewPath(char*, char*, char*, char*);
public:
	Image();
	Image(const Image&);
	Image& operator= (const Image&);
	virtual ~Image();
public:
	virtual void genGrayscale(std::ifstream&) = 0;
	virtual void genMonochrome(std::ifstream&) = 0;
	virtual void genRedHistogram(std::ifstream&) = 0;
	virtual void genGreenHistogram(std::ifstream&) = 0;
	virtual void genBlueHistogram(std::ifstream&) = 0;
};

