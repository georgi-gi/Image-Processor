#include "PixelColour.h"

void PixelColour::setRed(unsigned char red)
{
	this->red = red;
}

void PixelColour::setGreen(unsigned char green)
{
	this->green = green;
}

void PixelColour::setBlue(unsigned char blue)
{
	this->blue = blue;
}

unsigned char PixelColour::getRed()
{
	return red;
}

unsigned char PixelColour::getGreen()
{
	return green;
}

unsigned char PixelColour::getBlue()
{
	return blue;
}