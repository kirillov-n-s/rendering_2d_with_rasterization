#include "bresenham.h"

namespace Rasterization {

void bresenhamLine(
    const Line &line,
	const Color color,
	Bitmap& bitmap)
{
    int x1 = line.first.x;
    int y1 = line.first.y;
    const int x2 = line.second.x;
    const int y2 = line.second.y;

    const int dx = std::abs(x2 - x1);
    const int dy = std::abs(y2 - y1);
    const int sx = x1 < x2 ? 1 : -1;
    const int sy = y1 < y2 ? 1 : -1;
    int e = dx - dy;

    bitmap.setPixel(x2, y2, color);

    while (x1 != x2 || y1 != y2)
    {
        bitmap.setPixel(x1, y1, color);
        int e2 = e * 2;
        if (e2 > -dy)
        {
            e -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            e += dx;
            y1 += sy;
        }
    }
}
}
