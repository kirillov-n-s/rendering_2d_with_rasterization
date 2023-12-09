#pragma once

#include <array>
#include "../common_types.h"
#include "bitmap.h"

namespace Rasterization {

using Triangle = std::array<PixelCoord3d, 3>;

void scanlineTriangle(
	const Triangle& triangle,
	const Color color,
	Bitmap& bitmap,
	Bitmap& zBuffer);
}
