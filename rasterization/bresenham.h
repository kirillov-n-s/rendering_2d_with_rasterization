#pragma once

#include <glm/vec2.hpp>
#include "bitmap.h"

namespace Rasterization {

void bresenhamLine(
	const std::pair<glm::ivec2, glm::ivec2> &segment,
	const Color color,
	Bitmap &bitmap);
}
