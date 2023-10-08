#include "bitmap.h"

namespace Rasterization {

Bitmap::Bitmap(const int width, const int height)
	: m_width(width), m_height(height), m_pixels(width * height, Color{})
{}

Bitmap::Bitmap(const int width, const int height, const Color color)
	: m_width(width), m_height(height), m_pixels(width * height, color)
{}

int Bitmap::size() const
{
	return m_width * m_height;
}

int Bitmap::width() const
{
	return m_width;
}

int Bitmap::height() const
{
	return m_height;
}

const byte *Bitmap::data() const
{
	return (byte *)m_pixels.data();
}

void Bitmap::setPixel(const int x, const int y, const Color color)
{
	if (x < 0 || x >= width())
		return;
	if (y < 0 || y >= height())
		return;
	const int flatInd = x + (m_height - y - 1) * m_width;
	m_pixels[flatInd] = color;
}

void Bitmap::clear(const Color color)
{
	std::fill(m_pixels.begin(), m_pixels.end(), color);
}
}
