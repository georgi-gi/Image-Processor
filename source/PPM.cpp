#include "PPM.h"

PPM::PPM()
{

}

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
PPM::PPM(char type[], unsigned int width, unsigned int height, unsigned int maxVal, unsigned int endOfHeader, char* path) : Image()
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
		bitMap = new PixelColour*[height];
		for (int i = 0; i < height; ++i)
		{
			bitMap[i] = new PixelColour[width];
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
PPM::PPM(const PPM& o) : Image(o)
{
	bitMap = new PixelColour*[o.height];
	for (size_t row = 0; row < height; ++row)
	{
		bitMap[row] = new PixelColour[width];
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
///		референция към обекта, от който се присвояват данните
///
PPM& PPM::operator= (const PPM& o)
{
	if (this != &o)
	{
		for (int i = 0; i < height; ++i)
		{
			delete[] bitMap[i];
		}
		delete[] bitMap;

		Image::operator=(o);

		bitMap = new PixelColour*[o.height];
		for (size_t row = 0; row < height; ++row)
		{
			bitMap[row] = new PixelColour[width];
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

///
/// Деструктор.
/// Освобождава паметта, заета от двумерния масив от пиксели.
///
PPM::~PPM()
{
	for (int i = 0; i < height; ++i)
	{
		delete[] bitMap[i];
	}
	delete[] bitMap;
}

///
///	Преизчислява и променя стойностите в масива.
/// За всеки пиксел изчислява стойността на сивото, която се получава по формулата:
///	(0.3 * red) + (0.59 * green) + (0.11 * blue)
/// След като изчисли стойността на сивото, я слага в червеното, зеленото и синьото на съответния пиксел.
///
void PPM::turnGray()
{
	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			unsigned char gray = 0.3 * bitMap[row][col].getRed() +
								 0.59 * bitMap[row][col].getGreen() +
								 0.11 * bitMap[row][col].getBlue();

			bitMap[row][col].setRed(gray);
			bitMap[row][col].setGreen(gray);
			bitMap[row][col].setBlue(gray);
		}
	}
	return;
}

///
///	Променя стойностите на вече направеното grayscale изображение. Тъй като стойностите за red, green и blue
///са еднакви, няма значение коя от тях ще изберем за проверката. Ако стойността е по-голяма от maxVal/2,
///приравняваме стойностите в пиксела на maxVal (бяло). В противен случай ги приравняваме на 0 (черно).
///
void PPM::turnMono()
{
	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			if (bitMap[row][col].getRed() > maxVal/2)
			{
				bitMap[row][col].setRed(maxVal);
				bitMap[row][col].setGreen(maxVal);
				bitMap[row][col].setBlue(maxVal);
			}
			else
			{
				bitMap[row][col].setRed(0);
				bitMap[row][col].setGreen(0);
				bitMap[row][col].setBlue(0);
			}
		}
	}
	return;
}

///
///	Булева функция, която проверява дали подаденото изображение е grayscale.
/// Брои всички пиксели, които имат еднакви стойности за червено, зелено и синьо и проверява дали броят им
///е равен на броя на пикселите в изображението. Ако е равен, изображението е grayscale и връща true.
///В противен случай връща false - изображението не е grayscale, т.е. е цветно.
///
bool PPM::isGrayscale()
{
	int grayPixels = 0;

	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			if (bitMap[row][col].getRed() == bitMap[row][col].getGreen() &&
				bitMap[row][col].getGreen() == bitMap[row][col].getBlue())
			{
				++grayPixels;
			}
		}
	}

	//if all the pixels are gray
	if (grayPixels == (width * height))
	{
		std::cout << "The image is already grayscale!\n";
		return true;
	}
	else 
		return false;
}

///
///	Функцията проверява дали подаденото изображение е монохромно.
/// Ако срещне пиксел, в който поне едната стойност е различна от максималната или от 0, това значи, че
///изображението не е монохромно и връща false. Ако се извъртят циклите, значи изображението е монохромно
///и връща true.
///
bool PPM::isMonochrome()
{
	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			if ((bitMap[row][col].getRed() != maxVal ||
				bitMap[row][col].getGreen() != maxVal ||
				bitMap[row][col].getBlue() != maxVal) &&
				(bitMap[row][col].getRed() != 0 ||
				bitMap[row][col].getGreen() != 0 ||
				bitMap[row][col].getBlue() != 0))
			{
				return false;
			}
		}
	}

	return true;
}