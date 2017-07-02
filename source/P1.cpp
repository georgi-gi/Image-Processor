#include "P1.h"

P1::P1(char type[], unsigned int width, unsigned int height, unsigned int endOfHeader, char* path) : PBM(type, width, height, endOfHeader, path)
{

}

P1::P1(void)
{
}

P1::~P1(void)
{
}

void P1::fill(std::ifstream& image)
{
	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			bool value;
			image >> value;
			bitMap[row][col].setValue(value);
		}
	}
	return;
}

void P1::createNewImage(std::ofstream& image, char* newPath)
{
	image << type[0] << type[1] << '\n';
	image << width << ' ' << height << '\n';

	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			image << bitMap[row][col].getValue() << ' ';
		}
		image << '\n';
	}
	return;
}

void P1::genGrayscale(std::ifstream&)
{
	std::cout << "Cannot convert the monochrome image" << filePath << " to grayscale.\n";
	return;
}

void P1::genMonochrome(std::ifstream&)
{
	std::cout << filePath << " is already monochrome.\n";
	return;
}

///
///	Виртуална функция, която прави хистограма с ширина 255 и височина 100, като рисува колони в първите
///два реда пиксели толкова високи, колкото процента от изображението имат стойност 0 или 1.
///
void P1::genRedHistogram(std::ifstream& image)
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
	double* percentagesAcc = new double[maxVal + 1];
	int* percentages = new int[maxVal + 1];
	int* values = new int[maxVal + 1];
	for (int i = 0; i <= maxVal; ++i)
	{
		percentagesAcc[i] = 0.0;
		percentages[i] = 0;
		values[i] = 0;
	}
	for (size_t value = 0; value <= maxVal; ++value)
	{
		for (size_t row = 0; row < height; ++row)
		{
			for (size_t col = 0; col < width; ++col)
			{
				if (bitMap[row][col].getValue() == value)
					++values[value];
			}
		}
		///изчислява процентите на всяка стойност на сивото
		///percentages[5] = 3 : 3% от пикселите имат стойност 5 в сивото
		percentagesAcc[value] = (100 * (double)values[value])/(double)numberOfPixels;
		percentagesAcc[value] += 0.5;
		percentages[value] = (int)percentagesAcc[value];
	}

	char extension[] = ".pbm";
	char word[] = ".histogram";

	char* newFilePath = new char[strlen(filePath) + strlen(word) + 1];

	createNewPath(filePath, newFilePath, extension, word);

	newFilePath[strlen(newFilePath)] = '\0';

	P1 histogram("P1", 255, 100, endOfHeader, newFilePath);

	for (size_t row = 0; row < 100; ++row)
	{
		for (size_t col = 0; col < 2; ++col)
		{
			if (row < 100 - percentages[col])
			{
				histogram.bitMap[row][col].setValue(0);
			}
			else
			{
				histogram.bitMap[row][col].setValue(1);
			}
		}
	}

	for (size_t row = 0; row < 100; ++row)
	{
		for (size_t col = 2; col < 255; ++col)
		{
			histogram.bitMap[row][col].setValue(0);
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

void P1::genGreenHistogram(std::ifstream& image)
{
	genRedHistogram(image);
	return;
}

void P1::genBlueHistogram(std::ifstream& image)
{
	genRedHistogram(image);
	return;
}