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

	int width() const;
	int height() const;

	void setPixel(const int x, const int y, const Color color);

	const byte* data() const;
	void clear(const Color color);

private:
	int m_width = 0;
	int m_height = 0;
	int m_pixelSize = 1;
	std::vector<Color> m_pixels;
};
}
