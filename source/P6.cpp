#include "P6.h"

P6::P6() : PPM()
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
P6::P6(char type[], unsigned int width, unsigned int height, unsigned int maxVal, unsigned int endOfHeader, char* path) : PPM(type, width, height, maxVal, endOfHeader, path)
{

}

///
/// Копиращ конструктор
///
P6::P6(const P6& o) : PPM(o)
{

}

///
///	Оператор за присвояване
///	\param о
///		референция към обекта, от който се присвояват данните
///
P6& P6::operator=(const P6& o)
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
P6::~P6()
{

}

///
/// Виртуална функция, която чете данните от файла бинарно (такъв е формата на изображението).
/// Има три локални променливи от тип unsigned char, в които се прочитат стойностите на червеното,
///зеленото и синьото и след това се присвояват на сътветния пиксел в масива от пиксели на обекта.
///
void P6::fill(std::ifstream& image)
{
	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			unsigned char red;
			unsigned char green;
			unsigned char blue;

			image.read((char*)&red, sizeof(red));
			bitMap[row][col].setRed(red);
			image.read((char*)&green, sizeof(green));
			bitMap[row][col].setGreen(green);
			image.read((char*)&blue, sizeof(blue));
			bitMap[row][col].setBlue(blue);
		}
	}
	return;
}

///
///	Виртуална функция, която създава новия файл. Първо записва хедъра и след това стойност по стойност записва
///бинарно всеки пиксел.
///
void P6::createNewImage(std::ofstream& image, char* newFilePath)
{
	image << type[0] << type[1] << '\n' 
			   << width << ' ' << height << '\n' 
			   << maxVal << '\n';

	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			unsigned char red = (unsigned)bitMap[row][col].getRed();
			unsigned char green = (unsigned)bitMap[row][col].getGreen();
			unsigned char blue = (unsigned)bitMap[row][col].getBlue();

			image.write((char*)&red, sizeof(red));
			image.write((char*)&green, sizeof(green));
			image.write((char*)&blue, sizeof(blue));
		}
	}
	return;
}

///
///	Виртуална функция, която генерира новия grayscale файл.
///
void P6::genGrayscale(std::ifstream& image)
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
/// Виртуална функция, която генерира новото монохромно изображение.
///
void P6::genMonochrome(std::ifstream& readImage)
{
	if(!readImage)
	{
		std::cout << "There is a problem with the file. Aborting mission!\n";
		return;
	}
	std::cout << "Creating monochome...\n";
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

///
/// Виртуална функция, която генерира червената хистограма на подаденото изображение.
///
void P6::genRedHistogram(std::ifstream& readImage)
{
	if(!readImage)
	{
		std::cout << "There is a problem with the file. Aborting mission!\n";
		return;
	}
	std::cout << "Creating red histogram...\n";
	readImage.seekg(endOfHeader);
	fill(readImage);

	int numberOfPixels = width * height;
	//Acc = accurate
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

	P6 redHistogram("P6", 255, 100, 255, endOfHeader, newFilePath);

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
void P6::genGreenHistogram(std::ifstream& readImage)
{
	if(!readImage)
	{
		std::cout << "There is a problem with the file. Aborting mission!\n";
		return;
	}
	std::cout << "Creating green histogram...\n";
	readImage.seekg(endOfHeader);
	fill(readImage);

	int numberOfPixels = width * height;
	double percentagesAcc[255] = {0.0,};
	int percentages[255] = {0,};

	///redValues[5] = 15 : 15 пиксела със стойност на червеното 5
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

	P6 greenHistogram("P6", 255, 100, 255, endOfHeader, newFilePath);

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
void P6::genBlueHistogram(std::ifstream& readImage)
{
	if(!readImage)
	{
		std::cout << "There is a problem with the file. Aborting mission!\n";
		return;
	}
	std::cout << "Creating blue histogram...\n";
	readImage.seekg(endOfHeader);
	fill(readImage);

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

	P6 blueHistogram("P6", 255, 100, 255, endOfHeader, newFilePath);

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