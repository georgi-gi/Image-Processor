#pragma once

///
/// Клас PixelColour, който представлява пиксел на изображение с разширение .ppm. Съдържа три променливи
///от тип unsigned char (red, green, blue), които съхраняват стойността съответно на червеното, зеленото
///и синьото.
///
class PixelColour
{
private:
	unsigned char red;
	unsigned char green;
	unsigned char blue;
public:
	void setRed(unsigned char);
	void setGreen(unsigned char);
	void setBlue(unsigned char);
	unsigned char getRed();
	unsigned char getGreen();
	unsigned char getBlue();
};

