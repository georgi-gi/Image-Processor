#pragma once

///
/// Клас PixelGray, който представлява пиксел на изображение с разширение .pgm. Съдържа една променлива
///от тип unsigned char, която съхранява стойността на сивото.
///
class PixelGray
{
private:
	unsigned char gray;
public:
	void setGray(unsigned char);
	unsigned char getGray();
};

