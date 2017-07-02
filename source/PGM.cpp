#include "PGM.h"

///
/// Конструктор
/// \param type[]
///		Символният низ, който съдържа типа на подаденото изображение
///	\param width
///		Ширината на подаденото изображение (брой пиксели)
/// \param height
///		Височината на подаденото изображение (брой пиксели)
/// \param maxVal
///		Максималната стойност в изображението
/// \param path
///		Символен низ, който съдържа пълния път на подаденото изображение
///
PGM::PGM(char type[], unsigned int width, unsigned int height, unsigned int maxVal, unsigned int endOfHeader, char* path) : Image()
{
	bitMap = NULL;

	this->filePath = new char[strlen(path) + 1];
	strcpy(this->filePath, path);

	for (int i = 0; i < 3; ++i)
		this->type[i] = type[i];
	this->width = width;
	this->height = height;
	this->maxVal = maxVal;
	this->endOfHeader = endOfHeader;

	try
	{
		bitMap = new PixelGray*[height];
		for (int i = 0; i < height; ++i)
		{
			bitMap[i] = new PixelGray[width];
		}
	}
	catch(std::bad_alloc&)
	{
		std::cerr << "Couldn't allocate memory.\n";
	}
}

///
/// Копиращ конструктор
///
PGM::PGM(const PGM& o) : Image(o)
{
	bitMap = new PixelGray*[o.height];
	for (size_t row = 0; row < height; ++row)
	{
		bitMap[row] = new PixelGray[width];
	}
	
	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			bitMap[row][col] = o.bitMap[row][col];
		}
	}
}

///
///	Оператор за присвояване
///	\param о
///		референция към обекта, от когото се присвояват данните
///
PGM& PGM::operator= (const PGM& o)
{
	if (this != &o)
	{
		for (int i = 0; i < height; ++i)
		{
			delete[] bitMap[i];
		}
		delete[] bitMap;

		Image::operator=(o);

		bitMap = new PixelGray*[o.height];
		for (size_t row = 0; row < height; ++row)
		{
			bitMap[row] = new PixelGray[width];
		}
	
		for (size_t row = 0; row < height; ++row)
		{
			for (size_t col = 0; col < width; ++col)
			{
				bitMap[row][col] = o.bitMap[row][col];
			}
		}
	}

	return *this;
}

PGM::PGM(void)
{
}

///
/// Деструктор.
/// Освобождава паметта, заета от двумерния масив от пиксели.
///
PGM::~PGM(void)
{
	for (int i = 0; i < height; ++i)
	{
		delete[] bitMap[i];
	}
	delete[] bitMap;
}

///
///	Преизчислява и променя стойностите в масива, за да конвертира изображението в монохромно.
/// За всеки пиксел проверява дали стойността на сивото е по-голяма от максималната стойност, разделена
///на 2. Ако е по-голяма, слага стойност на сивото maxVal. В противен случай слага 0.
///
void PGM::turnMono()
{
	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			if (bitMap[row][col].getGray() > maxVal/2)
				bitMap[row][col].setGray(maxVal);

			else
				bitMap[row][col].setGray(0);
		}
	}
	return;
}
///
///	Функцията проверява дали подаденото изображение е монохромно.
/// Ако срещне пиксел, в който поне едната стойност е различна от максималната или от 0, това значи, че
///изображението не е монохромно и връща false. Ако се извъртят циклите, значи изображението е монохромно
///и връща true.
///
bool PGM::isMono()
{
	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			if (bitMap[row][col].getGray() != 0 && bitMap[row][col].getGray() != maxVal)
				return false;
		}
	}

	return true;
}