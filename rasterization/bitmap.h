#pragma once

#include <vector>
#include "color.h"

namespace Rasterization {

class Bitmap
{
public:
	Bitmap(const int width, const int height, const int pixelSize = 1);
	Bitmap(
		const int width, const int height,
		const Color color, const int pixelSize = 1);
	Bitmap(const Bitmap& other);

	static Bitmap fromExtents(const Bitmap& other);

	int width() const;
	int height() const;
	int physicalSize() const;

	Color getPixel(const int x, const int y, const Color outOfBoundsColor = colorBlack) const;
	void setPixel(const int x, const int y, const Color color);

	const int *data() const;
	void clear(const Color color);

	void floatMultiplySelf(const Bitmap& other);

private:
	int m_width = 0;
	int m_height = 0;
	int m_pixelSize = 1;
	std::vector<Color> m_pixels;
};
}
