#include "Image.h"


Image::Image()
{

}

Image::~Image()
{
	delete[] filePath;
}

///
/// Копиращ конструктор на обект от тип Image. Копира пътя на изображението, типа, ширината, височината
///и максималната стойност.
///
Image::Image(const Image& o)
{
	this->filePath = new char[strlen(o.filePath)];
	strcpy(this->filePath, o.filePath);
	this->type[0] = o.type[0];
	this->type[1] = o.type[1];
	this->type[2] = o.type[2];
	this->width = o.width;
	this->height = o.height;
	this->maxVal = o.maxVal;
	this->endOfHeader = o.endOfHeader;
}

///
/// Оператор за присвояване на обект от тип Image
///
Image& Image::operator= (const Image& o)
{
	if (this != &o)
	{
		delete[] filePath;

		this->filePath = new char[strlen(o.filePath)];
		strcpy(this->filePath, o.filePath);
		this->type[0] = o.type[0];
		this->type[1] = o.type[1];
		this->type[2] = o.type[2];
		this->width = o.width;
		this->height = o.height;
		this->endOfHeader = o.endOfHeader;
	}

	return *this;
}

///
/// Фунцкия, която генерира името на новия файл, който ще се получи след подадената команда. Разширението
///се запазва като се вмъква думата, съответстваща на командата между него и досегашното име.
///
void Image::createNewPath(char* oldPath, char* newPath, char* extension, char* word)
{
	size_t sizeOfWord = strlen(word);
	size_t sizeOfExtension = strlen(extension);
//	char* newFilePath = new char[strlen(oldPath) + strlen(word)];

	size_t sizeOfOldPath = strlen(oldPath);
	size_t sizeOfNewPath = strlen(oldPath) + strlen(word);

	for (size_t i = 0; i < sizeOfOldPath - 4; ++i)
	{
		newPath[i] = oldPath[i];
	}
	for (size_t i = sizeOfOldPath - 4, j = 0; j < sizeOfWord; ++i, ++j)
	{
		newPath[i] = word[j];
	}
	for (size_t i = sizeOfNewPath - 4, j = 0; i < sizeOfNewPath; ++i, ++j)
	{
		newPath[i] = extension[j];
	}
	newPath[sizeOfNewPath] = '\0';
	return;
}