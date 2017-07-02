#include "P4.h"

P4::P4(char type[], unsigned int width, unsigned int height, unsigned int endOfHeader, char* path) : PBM(type, width, height, endOfHeader, path)
{

}

P4::P4(const P4& o)
{

}

P4::P4(void)
{
}

P4::~P4(void)
{
}

void P4::fill(std::ifstream& image)
{
	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; col += 8)
		{
			char mask = 1;
			char num;
			image >> num;

			for (size_t i = col; i < col + 8; ++i)
			{
				if (num & mask << i)
					bitMap[row][i].setValue(true);
				else
					bitMap[row][i].setValue(false);
			}

		}
	}
	return;
}

void P4::createNewImage(std::ofstream& image, char* newPath)
{
	image << type[0] << type[1] << '\n' 
		  << width << ' ' << height << '\n';

	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; col += 8)
		{
			char num = 0;
			for (size_t i = col; i < col + 8; ++i)
			{
				int shifter = 7;
				if (bitMap[row][i].getValue() == true)
					num |= (1 << shifter);
				--shifter;
			}
			image.write((char*)& num, sizeof(num));
		}
	}
	return;
}

void P4::genGrayscale(std::ifstream&)
{
	std::cout << "Cannot convert the monochrome image" << filePath << " to grayscale.\n";
	return;
}

void P4::genMonochrome(std::ifstream&)
{
	std::cout << filePath << " is already monochrome.\n";
	return;
}

void P4::genRedHistogram(std::ifstream& image)
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

	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			if (bitMap[row][col].getValue() == false)
				++values[0];
		}
	}

	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			if (bitMap[row][col].getValue() == true)
				++values[1];
		}
	}
	///изчислява процентите на всяка стойност на сивото
	///percentages[5] = 3 : 3% от пикселите имат стойност 5 в сивото
	percentagesAcc[0] = (100 * (double)values[0])/(double)numberOfPixels;
	percentagesAcc[0] += 0.5;
	percentages[0] = (int)percentagesAcc[0];

	percentagesAcc[1] = (100 * (double)values[1])/(double)numberOfPixels;
	percentagesAcc[1] += 0.5;
	percentages[1] = (int)percentagesAcc[1];


	char extension[] = ".pbm";
	char word[] = ".histogram";

	char* newFilePath = new char[strlen(filePath) + strlen(word) + 1];

	createNewPath(filePath, newFilePath, extension, word);

	newFilePath[strlen(newFilePath)] = '\0';

	P4 histogram("P4", 255, 100, endOfHeader, newFilePath);

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

void P4::genGreenHistogram(std::ifstream& image)
{
	genRedHistogram(image);
	return;
}

void P4::genBlueHistogram(std::ifstream& image)
{
	genRedHistogram(image);
	return;
}