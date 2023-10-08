#pragma once

#include <vector>
#include "color.h"

namespace Rasterization {

class Bitmap
{
public:
	Bitmap(const int width, const int height);
	Bitmap(const int width, const int height, const Color color);

	int size() const;
	int width() const;
	int height() const;

	Color pixel(const int x, const int y) const;
	void setPixel(const int x, const int y, const Color color);

	const byte* data() const;
	void clear(const Color color);

private:
	int m_width = 0;
	int m_height = 0;
	std::vector<Color> m_pixels;

	bool inBounds(const int x, const int y) const;
};
}
