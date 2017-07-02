#include "P3.h"


P3::P3() : PPM()
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
P3::P3(char type[], unsigned int width, unsigned int height, unsigned int maxVal, unsigned int endOfHeader, char* path) : PPM(type, width, height, maxVal, endOfHeader, path)
{

}

///
/// Копиращ конструктор
///
P3::P3(const P3& o) : PPM(o)
{

}

///
///	Оператор за присвояване
///	\param о
///		референция към обекта, от който се присвояват данните
///
P3& P3::operator=(const P3& o)
{
	if (this != &o)
	{
		PPM::operator=(o);
	}
	return *this;
}

///
///	Деструктор
///
P3::~P3(void)
{

}

///
/// Виртуална функция, която чете данните от файла текстово (такъв е формата на изображението).
/// Има три локални променливи от тип unsigned int (ако е char, ще прочете първия символ от числото),
///в които се прочитат стойностите на червеното, зеленото и синьото и след това се присвояват на 
///сътветния пиксел в масива от пиксели на обекта.
///	В началото на всеки нов ред, както и в края на всеки, се прави проверка за коментари и се пропускат.
///
void P3::fill(std::ifstream& image)
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
			unsigned int red;
			unsigned int green;
			unsigned int blue;

			image >> red;
			bitMap[row][col].setRed(red);
			image >> green;
			bitMap[row][col].setGreen(green);
			image >> blue;
			bitMap[row][col].setBlue(blue);

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
///като текст всеки пиксел (един пиксел е поредица от 3 числа).
///
void P3::createNewImage(std::ofstream& newImage, char* newFilePath)
{
	newImage << type[0] << type[1] << '\n' 
			   << width << ' ' << height << '\n' 
			   << maxVal << '\n';

	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			newImage << (unsigned)bitMap[row][col].getRed() << ' '
					   << (unsigned)bitMap[row][col].getGreen() << ' '
					   << (unsigned)bitMap[row][col].getBlue() << ' ';
		}
		newImage << '\n';
	}
	return;
}

///
///	Виртуална функция, която генерира новия grayscale файл.
///
void P3::genGrayscale(std::ifstream& image)
{
	if(!image)
	{
		std::cout << "There is a problem with the file. Aborting mission!\n";
		return;
	}
	std::cout << "Creating grayscale...\n";
	image.seekg(endOfHeader);
	fill(image);

	if (isGrayscale() == true)
		return;
	
	turnGray();

	char extension[] = ".ppm";
	char word[] = ".grayscale";

	char* newFilePath = new char[strlen(filePath) + strlen(word) + 1];

	createNewPath(filePath, newFilePath, extension, word);

	newFilePath[strlen(newFilePath)] = '\0';

	std::ofstream grayscaled(newFilePath, std::ios::binary);

	createNewImage(grayscaled, newFilePath);

	grayscaled.close();

	delete[] newFilePath;
	std::cout << "Grayscale created\n";
	return;
}

///
/// Виртуална функция, която генерира червената хистограма на подаденото изображение.
///
void P3::genRedHistogram(std::ifstream& image)
{
	if(!image)
	{
		std::cout << "There is a problem with the file. Aborting mission!\n";
		return;
	}
	std::cout << "Creating red histogram...\n";
	image.seekg(endOfHeader);
	fill(image);

	int numberOfPixels = width * height;
	double percentagesAcc[255] = {0.0,};
	int percentages[255] = {0,};

	///redValues[5] = 15 : 15 пиксела със стойност на червеното 5
	int redValues[255] = {0,};
	for (size_t value = 0; value < 255; ++value)
	{
		for (size_t row = 0; row < height; ++row)
		{
			for (size_t col = 0; col < width; ++col)
			{
				if (bitMap[row][col].getRed() == value)
					++redValues[value];
			}
		}
		///изчислява процентите на всяка стойност на червеното
		///percentages[5] = 3 : 3% от пикселите имат стойност 5 на червеното
		percentagesAcc[value] = (100 * (double)redValues[value])/(double)numberOfPixels;
		percentagesAcc[value] += 0.5;
		percentages[value] = (int)percentagesAcc[value];
	}

	char extension[] = ".ppm";
	char word[] = ".redHistogram";

	char* newFilePath = new char[strlen(filePath) + strlen(word) + 1];

	createNewPath(filePath, newFilePath, extension, word);

	newFilePath[strlen(newFilePath)] = '\0';

	P3 redHistogram("P3", 255, 100, maxVal,endOfHeader, newFilePath);

	for (size_t row = 0; row < 100; ++row)
	{
		for (size_t col = 0; col < 255; ++col)
		{
			if (row < 100 - percentages[col])
			{
				redHistogram.bitMap[row][col].setRed(255);
				redHistogram.bitMap[row][col].setGreen(255);
				redHistogram.bitMap[row][col].setBlue(255);
			}
			else
			{
				redHistogram.bitMap[row][col].setRed(255);
				redHistogram.bitMap[row][col].setGreen(0);
				redHistogram.bitMap[row][col].setBlue(0);
			}
		}
	}

	std::ofstream redHisto(newFilePath, std::ios::binary);
	
	redHistogram.createNewImage(redHisto, newFilePath);
	
	redHisto.close();

	delete[] newFilePath;
	std::cout << "Red histogram created\n";
	return;
}

///
/// Виртуална функция, която генерира зелената хистограма на подаденото изображение
///
void P3::genGreenHistogram(std::ifstream& image)
{
	if(!image)
	{
		std::cout << "There is a problem with the file. Aborting mission!\n";
		return;
	}
	std::cout << "Creating green histogram...\n";
	image.seekg(endOfHeader);
	fill(image);

	int numberOfPixels = width * height;
	int percentages[255] = {0,};

	///redValues[5] = 15 : 15 пиксела със стойност на червеното 5
	double percentagesAcc[255] = {0.0,};
	int greenValues[255] = {0,};
	for (size_t value = 0; value < 255; ++value)
	{
		for (size_t row = 0; row < height; ++row)
		{
			for (size_t col = 0; col < width; ++col)
			{
				if (bitMap[row][col].getGreen() == value)
					++greenValues[value];
			}
		}
		///изчислява процентите на всяка стойност на червеното
		///percentages[5] = 3 : 3% от пикселите имат стойност 5 на червеното
		percentagesAcc[value] = (100 * (double)greenValues[value])/(double)numberOfPixels;
		percentagesAcc[value] += 0.5;
		percentages[value] = (int)percentagesAcc[value];
	}

	char extension[] = ".ppm";
	char word[] = ".greenHistogram";

	char* newFilePath = new char[strlen(filePath) + strlen(word) + 1];

	createNewPath(filePath, newFilePath, extension, word);

	newFilePath[strlen(newFilePath)] = '\0';

	P3 greenHistogram("P3", 255, 100, 255,endOfHeader, newFilePath);

	for (size_t row = 0; row < 100; ++row)
	{
		for (size_t col = 0; col < 255; ++col)
		{
			if (row < 100 - percentages[col])
			{
				greenHistogram.bitMap[row][col].setRed(255);
				greenHistogram.bitMap[row][col].setGreen(255);
				greenHistogram.bitMap[row][col].setBlue(255);
			}
			else
			{
				greenHistogram.bitMap[row][col].setRed(0);
				greenHistogram.bitMap[row][col].setGreen(255);
				greenHistogram.bitMap[row][col].setBlue(0);
			}
		}
	}

	std::ofstream greenHisto(newFilePath, std::ios::binary);
	
	greenHistogram.createNewImage(greenHisto, newFilePath);
	
	greenHisto.close();

	delete[] newFilePath;
	std::cout << "Green histogram created\n";
	return;
}

///
///	Виртуална функция, която генерира синята хистограма на подаденото изображение
///
void P3::genBlueHistogram(std::ifstream& image)
{
	if(!image)
	{
		std::cout << "There is a problem with the file. Aborting mission!\n";
		return;
	}
	std::cout << "Creating blue histogram...\n";
	image.seekg(endOfHeader);
	fill(image);

	int numberOfPixels = width * height;
	double percentagesAcc[255] = {0.0,};
	int percentages[255] = {0,};

	///redValues[5] = 15 : 15 пиксела със стойност на червеното 5
	int blueValues[255] = {0,};
	for (size_t value = 0; value < 255; ++value)
	{
		for (size_t row = 0; row < height; ++row)
		{
			for (size_t col = 0; col < width; ++col)
			{
				if (bitMap[row][col].getBlue() == value)
					++blueValues[value];
			}
		}
		///изчислява процентите на всяка стойност на червеното
		///percentages[5] = 3 : 3% от пикселите имат стойност 5 на червеното
		percentagesAcc[value] = (100 * (double)blueValues[value])/(double)numberOfPixels;
		percentagesAcc[value] += 0.5;
		percentages[value] = (int)percentagesAcc[value];
	}

	char extension[] = ".ppm";
	char word[] = ".blueHistogram";

	char* newFilePath = new char[strlen(filePath) + strlen(word) + 1];

	createNewPath(filePath, newFilePath, extension, word);

	newFilePath[strlen(newFilePath)] = '\0';

	P3 blueHistogram("P3", 255, 100, 255, endOfHeader, newFilePath);

	for (size_t row = 0; row < 100; ++row)
	{
		for (size_t col = 0; col < 255; ++col)
		{
			if (row < 100 - percentages[col])
			{
				blueHistogram.bitMap[row][col].setRed(255);
				blueHistogram.bitMap[row][col].setGreen(255);
				blueHistogram.bitMap[row][col].setBlue(255);
			}
			else
			{
				blueHistogram.bitMap[row][col].setRed(0);
				blueHistogram.bitMap[row][col].setGreen(0);
				blueHistogram.bitMap[row][col].setBlue(255);
			}
		}
	}

	std::ofstream blueHisto(newFilePath, std::ios::binary);
	
	blueHistogram.createNewImage(blueHisto, newFilePath);
	
	blueHisto.close();

	delete[] newFilePath;
	std::cout << "Blue histogram created\n";
	return;
}

///
/// Виртуална функция, която генерира новото монохромно изображение.
///
void P3::genMonochrome(std::ifstream& readImage)
{
	if(!readImage)
	{
		std::cout << "There is a problem with the file. Aborting mission!\n";
		return;
	}
	std::cout << "Creating monochrome...\n";
	readImage.seekg(endOfHeader);
	fill(readImage);
	
	if(isMonochrome())
		return;

	if(!isGrayscale())
		turnGray();

	turnMono();

	char extension[] = ".ppm";
	char word[] = ".monochrome";
	
	char* newFilePath = new char[strlen(filePath) + strlen(word) + 1];

	createNewPath(filePath, newFilePath, extension, word);

	newFilePath[strlen(newFilePath)] = '\0';

	std::ofstream monochromed(newFilePath, std::ios::binary);

	createNewImage(monochromed, newFilePath);

	monochromed.close();

	delete[] newFilePath;
	std::cout << "Monochrome created\n";
	return;
}