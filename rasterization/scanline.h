#pragma once

#include <array>
#include "../common_types.h"
#include "bitmap.h"

namespace Rasterization {

using Triangle = std::array<PixelCoord2d, 3>;

void scanlineTriangle(
	const Triangle& triangle,
	const Color color,
	Bitmap& bitmap);
}
