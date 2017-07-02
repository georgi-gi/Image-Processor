#include "P2.h"

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
P2::P2(char type[], unsigned int width, unsigned int height, unsigned int maxVal, unsigned int endOfHeader, char* path) : PGM(type, width, height, maxVal, endOfHeader, path)
{

}

///
/// Копиращ конструктор
///
P2::P2(const P2& o) : PGM(o)
{

}

///
///	Оператор за присвояване
///	\param о
///		референция към обекта, от който се присвояват данните
///
P2& P2::operator= (const P2& o)
{
	if (this != &o)
	{
		PGM::operator=(o);
	}
	return *this;
}

P2::P2(void)
{
}

///
///	Деструктор
///
P2::~P2(void)
{
}

///
/// Виртуална функция, която чете данните от файла текстово (такъв е формата на изображението).
/// Има локална променлива от тип unsigned int (ако е char, ще прочете първия символ от числото),
///в който се прочита стойността на сивото и след това се присвоява на сътветния пиксел в масива 
///от пиксели на обекта.
///	В началото на всеки нов ред, както и в края на всеки, се прави проверка за коментари и се пропускат.
///
void P2::fill(std::ifstream& image)
{
	for (size_t row = 0; row < height; ++row)
	{
		char c;
		image.get(c);
		if (c == '#')
		{
			char ch;
			do
			{
			
				image.get(ch);
			}
			while (ch != '\n');
		}
		else image.unget();

		for (size_t col = 0; col < width; ++col)
		{
			unsigned int gray;
			image >> gray;
			bitMap[row][col].setGray(gray);

			if (col == width - 1)
			{
				image.get(c);
				if (c == '#')
				{
					char ch;
					do
					{
			
						image.get(ch);
					}
					while (ch != '\n');
				}
				else image.unget();
			}
		}
	}
	return;
}

///
///	Виртуална функция, която създава новия файл. Първо записва хедъра и след това стойност по стойност записва
///като текст всеки пиксел.
///
void P2::createNewImage(std::ofstream& image, char* path)
{
	image << type[0] << type[1] << '\n';
	image << width << ' ' << height << '\n';
	image << maxVal << '\n';

	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			image << (unsigned)bitMap[row][col].getGray() << ' ';
		}
		image << '\n';
	}
	return;
}

///
///	Виртуална функция, която изписва в конзолата че файлът е grayscale (.pgm е винаги такъв).
///
void P2::genGrayscale(std::ifstream& image)
{
	std::cout << filePath << " is already grayscale.\n";
	return;
}

///
/// Виртуална функция, която генерира новото монохромно изображение.
/// След като се прочете файла, се прави проверка дали изображението е вече монохромно.
///
void P2::genMonochrome(std::ifstream& image)
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
void P2::genRedHistogram(std::ifstream& image)
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

	P2 histogram("P2", maxVal, 100, maxVal, endOfHeader, newFilePath);

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
void P2::genGreenHistogram(std::ifstream& image)
{
	genRedHistogram(image);
	return;
}

///
///	Виртуална функция, която извиква функцията genRedHistogram(), тъй като файлът е безцветен.
///
void P2::genBlueHistogram(std::ifstream& image)
{
	genRedHistogram(image);
	return;
}