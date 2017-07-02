#include "P5.h"

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
P5::P5(char type[], unsigned int width, unsigned int height, unsigned int maxVal, unsigned int endOfHeader, char* path) : PGM(type, width, height, maxVal, endOfHeader, path)
{

}

///
/// Копиращ конструктор
///
P5::P5(const P5& o) : PGM(o)
{

}

///
///	Оператор за присвояване
///	\param о
///		референция към обекта, от който се присвояват данните
///
P5& P5::operator= (const P5& o)
{
	if (this != &o)
	{
		PGM::operator=(o);
	}
	return *this;
}

P5::P5(void)
{
}

///
///	Деструктор
///
P5::~P5(void)
{
}

///
/// Виртуална функция, която чете данните от файла бинарно (такъв е формата на изображението).
/// Има локална променлива от тип unsigned int (ако е char, ще прочете първия символ от числото),
///в който се прочита стойността на сивото и след това се присвоява на сътветния пиксел в масива 
///от пиксели на обекта.
///
void P5::fill(std::ifstream& image)
{
	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			unsigned char gray;
			image.read((char*)&gray, sizeof(gray));
			bitMap[row][col].setGray(gray);
		}
	}
	return;
}

///
///	Виртуална функция, която създава новия файл. Първо записва хедъра и след това стойност по стойност записва
///като текст всеки пиксел.
///
void P5::createNewImage(std::ofstream& image, char* newPath)
{
	image << type[0] << type[1] << '\n' 
			   << width << ' ' << height << '\n' 
			   << maxVal << '\n';

	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			unsigned char gray = (unsigned)bitMap[row][col].getGray();

			image.write((char*)&gray, sizeof(gray));
		}
	}
	return;
}

///
///	Виртуална функция, която изписва в конзолата че файлът е grayscale (.pgm е винаги такъв).
///
void P5::genGrayscale(std::ifstream&)
{
	std::cout << filePath << " is already grayscale.\n";
	return;
}

///
/// Виртуална функция, която генерира новото монохромно изображение.
/// След като се прочете файла, се прави проверка дали изображението е вече монохромно.
///
void P5::genMonochrome(std::ifstream& image)
{
	if(!image)
	{
		std::cout << "There is a problem with the file. Aborting mission!\n";
		return;
	}
	std::cout << "Creating monochrome...\n";
	image.seekg(endOfHeader);
	fill(image);

	if (isMono())
	{
		std::cout << filePath << " is already monochrome.\n";
		return;
	}

	turnMono();
	
	char extension[] = ".pgm";
	char word[] = ".monochrome";

	char* newFilePath = new char[strlen(filePath) + strlen(word) + 1];

	createNewPath(filePath, newFilePath, extension, word);

	newFilePath[strlen(newFilePath)] = '\0';

	std::ofstream monochrome(newFilePath, std::ios::binary);

	createNewImage(monochrome, newFilePath);

	monochrome.close();

	delete[] newFilePath;
	std::cout << "Monochrome created\n";
	return;
}

///
/// Виртуална функция, която прави хистограма по единствения канал на изображението (сив).
///
void P5::genRedHistogram(std::ifstream& image)
{
	if(!image)
	{
		std::cout << "There is a problem with the file. Aborting mission!\n";
		return;
	}
	std::cout << "Creating histogram...\n";
	image.seekg(endOfHeader);
	fill(image);

	int numberOfPixels = width * height;
	double* percentagesAcc = new double[maxVal];
	int* percentages = new int[maxVal];
	int* grayValues = new int[maxVal];
	for (int i = 0; i < maxVal; ++i)
	{
		percentagesAcc[i] = 0.0;
		percentages[i] = 0;
		grayValues[i] = 0;
	}
	for (size_t value = 0; value < maxVal; ++value)
	{
		for (size_t row = 0; row < height; ++row)
		{
			for (size_t col = 0; col < width; ++col)
			{
				if (bitMap[row][col].getGray() == value)
					++grayValues[value];
			}
		}
		//изчислява процентите на всяка стойност на сивото
		//percentages[5] = 3 : 3% от пикселите имат стойност 5 в сивото
		percentagesAcc[value] = (100 * (double)grayValues[value])/(double)numberOfPixels;
		percentagesAcc[value] += 0.5;
		percentages[value] = (int)percentagesAcc[value];
	}

	char extension[] = ".pgm";
	char word[] = ".histogram";

	char* newFilePath = new char[strlen(filePath) + strlen(word) + 1];

	createNewPath(filePath, newFilePath, extension, word);

	newFilePath[strlen(newFilePath)] = '\0';

	P5 histogram("P5", maxVal, 100, maxVal, endOfHeader, newFilePath);

	for (size_t row = 0; row < 100; ++row)
	{
		for (size_t col = 0; col < maxVal; ++col)
		{
			if (row < 100 - percentages[col])
			{
				histogram.bitMap[row][col].setGray(maxVal);
			}
			else
			{
				histogram.bitMap[row][col].setGray(0);
			}
		}
	}

	for (size_t row = 0; row < 100; ++row)
	{
		for (size_t col = maxVal; col < 255; ++col)
		{
			histogram.bitMap[row][col].setGray(0);
		}
	}

	std::ofstream histo(newFilePath, std::ios::binary);
	
	histogram.createNewImage(histo, newFilePath);
	
	histo.close();

	delete[] percentagesAcc;
	delete[] percentages;
	delete[] newFilePath;
	std::cout << "Histogram created\n";
	return;
}

///
///	Виртуална функция, която извиква функцията genRedHistogram(), тъй като файлът е безцветен.
///
void P5::genGreenHistogram(std::ifstream& image)
{
	genRedHistogram(image);
	return;
}

///
///	Виртуална функция, която извиква функцията genRedHistogram(), тъй като файлът е безцветен.
///
void P5::genBlueHistogram(std::ifstream& image)
{
	genRedHistogram(image);
	return;
}