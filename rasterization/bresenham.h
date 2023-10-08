#pragma once

#include "../common_types.h"
#include "bitmap.h"

namespace Rasterization {

using Line = std::pair<PixelCoord2d, PixelCoord2d>;

void bresenhamLine(
	const Line &line,
	const Color color,
	Bitmap &bitmap);
}
