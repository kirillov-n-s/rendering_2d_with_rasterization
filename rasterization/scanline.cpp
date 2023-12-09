#include <glm/vector_relational.hpp>
#include "scanline.h"

namespace Rasterization {

void scanlineTriangle(
    const Triangle& triangle,
    const Color color,
    Bitmap& bitmap,
    Bitmap& zBuffer)
{
    const PixelCoord3d zeroVec(0);
    const PixelCoord3d screenExtents(bitmap.width(), bitmap.height(), std::numeric_limits<int>::max());
    auto [t0, t1, t2] = triangle;
    if (glm::any(glm::lessThan(t0, zeroVec)) || glm::any(glm::greaterThanEqual(t0, screenExtents))
        || glm::any(glm::lessThan(t1, zeroVec)) || glm::any(glm::greaterThanEqual(t1, screenExtents))
        || glm::any(glm::lessThan(t2, zeroVec)) || glm::any(glm::greaterThanEqual(t2, screenExtents)))
        return;
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
    const int upperPartHeight = t2.y - t1.y;
    const int lowerPartHeight = t1.y - t0.y;
    for (int y = 0; y < triangleHeight; ++y) {
        const bool isUpperPart = hasNoLowerPart || y > lowerPartHeight;
        const int partHeight = isUpperPart ? upperPartHeight : lowerPartHeight;
        const float triangleHeightRatio = (float)y / triangleHeight;
        const int prevPartHeight = isUpperPart ? lowerPartHeight : 0;
        const float partHeightRatio = float(y - prevPartHeight) / partHeight;
        PixelCoord3d scanlineStart = Coord3d(t0) + Coord3d(t2 - t0) * triangleHeightRatio;
        PixelCoord3d scanlineEnd = isUpperPart
            ? Coord3d(t1) + Coord3d(t2 - t1) * partHeightRatio
            : Coord3d(t0) + Coord3d(t1 - t0) * partHeightRatio;
        if (scanlineStart.x > scanlineEnd.x)
            std::swap(scanlineStart, scanlineEnd);
        const PixelCoord3d scanlineVec = scanlineEnd - scanlineStart;
        for (int x = scanlineStart.x; x <= scanlineEnd.x; ++x) {
            const float scanlineRatio = scanlineEnd.x == scanlineStart.x
                ? 1.0f
                : float(x - scanlineStart.x) / scanlineVec.x;
            const int pixelX = x;
            const int pixelY = t0.y + y;
            const int depth = float(scanlineStart.z) + float(scanlineVec.z) * scanlineRatio;
            const int prevDepth = colorToInt(zBuffer.getPixel(pixelX, pixelY));
            if (prevDepth < depth) {
                zBuffer.setPixel(pixelX, pixelY, intToColor(depth));
                bitmap.setPixel(pixelX, pixelY, color);
            }
        }
    }
}
}
