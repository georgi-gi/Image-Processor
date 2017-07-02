#include "Image.h"
#include "P1.h"
#include "P2.h"
#include "P3.h"
#include "P4.h"
#include "P5.h"
#include "P6.h"


///
///	Целта на функцията е да прочете хедъра от подадения файл и да запише информацията във вече дефинирани
///променливи. Приема като параметри референции към потока (reader), char-масива, който съхранява типа на 
///изображението (type), и 3 променливи от тип unsigned int, които съхваняват ширината (width), височината
///(height)и максималната стойност (maxVal).
/// В случай, че изображението е от тип P1 или P4, които нямат максимална стойност, записана в хедъра,
///фунцкията спира и не записва нищо в променливата.
///
void readHeader(std::ifstream& reader, char*& type, unsigned int& width, unsigned int& height, unsigned int& maxVal, unsigned int& endOfHeader)
{
	do
	{
		char c;

		if (type[0] != 'P')
		{
			reader >> type[0] >> type[1];
			type[2] = '\0';
			continue;
		}
		reader.get(c);
		if (c == '\n')
			continue;
		else reader.unget();

		if (c == '#')
		{
			char ch;
			do
			{
			
				reader.get(ch);
			}
			while (ch != '\n');
			continue;
		}

		if (width == 0)
		{
			char ch;
			do
			{
				reader.get(ch);
			}
			while (ch < '1' || ch > '9');
			reader.unget();
			reader >> width;
		}
		if (height == 0)
		{
			char ch;
			do
			{
				reader.get(ch);
			}
			while (ch < '1' || ch > '9');
			reader.unget();
			reader >> height;
		}

		reader.get(c);
		if (c == '\n')
			continue;

		if (maxVal == 0 && type[1] != '1' && type[1] != '4')
		{
			reader.unget();
			reader >> maxVal;
			break;
		}

		if (maxVal == 0 && (type[1] == '1' || type[1] == '4'))
			break;

		if (maxVal != 0)
			break;

		reader.get(c);
		if (c == '\n')
			continue;
	}
	while (true);
	endOfHeader = reader.tellg();
}

Image* createImage(Image*& picture, char* type, unsigned int width, unsigned int height, unsigned int maxVal, unsigned int endOfHeader, char* path)
{
	switch (type[1])
	{
	case '1':
		return new P1(type, width, height, endOfHeader, path);
		break;
	case '2':
		return new P2(type, width, height, maxVal, endOfHeader, path);
		break;
	case '3':
		return new P3(type, width, height, maxVal, endOfHeader, path);
		break;
	case '4':
		return new P4(type, width, height, endOfHeader, path);
		break;
	case '5':
		return new P5(type, width, height, maxVal, endOfHeader, path);	
		break;
	case '6':
		return new P6(type, width, height, maxVal, endOfHeader, path);
		break;
	default:
		std::cout << "Unrecognized file type.\n";
		break;
	}
	return NULL;
}

void callCommand(Image*& picture, std::ifstream& imageRead, int argc, char** argv)
{
	for (int j = 1; j < argc; ++j)
	{
		if (argv[j][0] == '-' && argv[j][1] == '-')
		{
			if (strcmp(argv[j], "--grayscale") == 0)
				picture->genGrayscale(imageRead);

			else if (strcmp(argv[j], "--monochrome") == 0)
				picture->genMonochrome(imageRead);

			else if (strcmp(argv[j], "--histogram=red") == 0)
				picture->genRedHistogram(imageRead);

			else if (strcmp(argv[j], "--histogram=green") == 0)
				picture->genGreenHistogram(imageRead);

			else if (strcmp(argv[j], "--histogram=blue") == 0)
				picture->genBlueHistogram(imageRead);
			else 
				std::cout << argv[j] << " is unrecognized command.\n";
		}
	}
}

int main(int argc, char* argv[])
{
	std::ifstream imageRead;

	Image* picture;

	char* type = new char[3];
	unsigned int width = 0, height = 0, maxVal = 0, endOfHeader = 0;
	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] != '-' && argv[i][1] != '-')
		{
			imageRead.open(argv[i], std::ios::binary);
			if(!imageRead)
			{
				std::cout << argv[i] << " does not exist.\n";
				continue;
			}
			readHeader(imageRead, type, width, height, maxVal, endOfHeader);

			picture = createImage(picture, type, width, height, maxVal, endOfHeader, argv[i]);

			if (picture == NULL)
				continue;
			
			callCommand(picture, imageRead, argc, argv);

			delete picture;
		}	
		imageRead.close();
	}
	
	delete[] type;
	return 0;
}