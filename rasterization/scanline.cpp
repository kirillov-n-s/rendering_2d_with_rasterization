#include "scanline.h"

namespace Rasterization {

void scanlineTriangle(
	const Triangle& triangle,
	const Color color,
	Bitmap& bitmap)
{
    auto [t0, t1, t2] = triangle;
    if (t0.y > t1.y)
        std::swap(t0, t1);
    if (t0.y > t2.y)
        std::swap(t0, t2);
    if (t1.y > t2.y)
        std::swap(t1, t2);
    const bool hasNoLowerPart = t0.y == t1.y;
    const bool hasNoUpperPart = t1.y == t2.y;
    if (hasNoLowerPart && hasNoUpperPart)
        return;
    const int triangleHeight = t2.y - t0.y;
    int upperPartHeight = t2.y - t1.y;
    int lowerPartHeight = t1.y - t0.y;
    for (int y = 0; y < triangleHeight; ++y) {
        const bool isUpperPart = hasNoLowerPart || y > lowerPartHeight;
        const int partHeight = isUpperPart ? upperPartHeight : lowerPartHeight;
        const float triangleHeightRatio = (float)y / triangleHeight;
        const int prevPartHeight = isUpperPart ? lowerPartHeight : 0;
        const float partHeightRatio = (float)(y - prevPartHeight) / partHeight;
        PixelCoord2d scanlineStart = Coord2d(t0) + Coord2d(t2 - t0) * triangleHeightRatio;
        PixelCoord2d scanlineEnd = isUpperPart
            ? Coord2d(t1) + Coord2d(t2 - t1) * partHeightRatio
            : Coord2d(t0) + Coord2d(t1 - t0) * partHeightRatio;
        if (scanlineStart.x > scanlineEnd.x)
            std::swap(scanlineStart, scanlineEnd);
        for (int x = scanlineStart.x; x <= scanlineEnd.x; ++x)
            bitmap.setPixel(x, t0.y + y, color); // attention, due to int casts t0.y+i != A.y
    }
}
}
