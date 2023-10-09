#include "bitmap.h"

namespace Rasterization {

Bitmap::Bitmap(const int width, const int height, const int pixelSize)
	: m_width(width), m_height(height), m_pixelSize(pixelSize),
	  m_pixels(width * height * m_pixelSize * m_pixelSize, Color{})
{}

Bitmap::Bitmap(
	const int width, const int height,
	const Color color, const int pixelSize)
	: m_width(width), m_height(height), m_pixelSize(pixelSize),
	  m_pixels(width * height * m_pixelSize * m_pixelSize, color)
{}

int Bitmap::width() const
{
	return m_width * m_pixelSize;
}

int Bitmap::height() const
{
	return m_height * m_pixelSize;
}

const byte *Bitmap::data() const
{
	return (byte *)m_pixels.data();
}

void Bitmap::setPixel(const int x, const int y, const Color color)
{
	if (x < 0 || x >= m_width)
		return;
	if (y < 0 || y >= m_height)
		return;
	const int x1 = x;
	const int y1 = m_height - y - 1;
	const int x2 = m_pixelSize * x1;
	const int y2 = m_pixelSize * y1;
	const int w2 = m_width * m_pixelSize;
	for (int subpixelX = 0; subpixelX < m_pixelSize; ++subpixelX)
		for (int subpixelY = 0; subpixelY < m_pixelSize; ++subpixelY)
			m_pixels[(x2 + subpixelX) + (y2 + subpixelY) * w2] = color;
}

void Bitmap::clear(const Color color)
{
	std::fill(m_pixels.begin(), m_pixels.end(), color);
}
}
