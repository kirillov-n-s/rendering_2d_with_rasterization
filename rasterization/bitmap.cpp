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

Color Bitmap::pixel(const int x, const int y) const
{
	return m_pixels[x + y * m_width];
}

void Bitmap::setPixel(const int x, const int y, const Color color)
{
	if (!inBounds(x, y))
		return;
	m_pixels[x + y * m_width] = color;
}

void Bitmap::clear(const Color color)
{
	std::fill(m_pixels.begin(), m_pixels.end(), color);
}

bool Bitmap::inBounds(const int x, const int y) const
{
	const int flatInd = x + y * m_width;
	return flatInd >= 0 && flatInd < size();
}
}
