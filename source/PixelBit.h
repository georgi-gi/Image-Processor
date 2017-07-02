#pragma once

///
/// Клас PixelBit, който представлява пиксел на изображение с разширение .pbm. Съдържа една променлива
///от тип bool, която съхранява стойността пиксела в изображението, която в този формат е 0 или 1.
///
class PixelBit
{
	private:
	bool value;
public:
	bool getValue(void);
	void setValue(bool value);
};

