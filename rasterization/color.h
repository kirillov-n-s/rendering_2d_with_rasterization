#pragma once

#include <random>
#include <glm/vec3.hpp>

using byte = unsigned char;
using Coord3d = glm::vec3;

namespace Rasterization {

struct Color
{
	byte r = 0;
	byte g = 0;
	byte b = 0;
	byte a = 255;
};

const Color colorBlack{};
const Color colorRed{ .r = 255 };
const Color colorGreen{ .g = 255 };
const Color colorBlue{ .b = 255 };

const Color colorYellow{
	.r = 255,
	.g = 255,
};
const Color colorCyan{
	.g = 255,
	.b = 255,
};
const Color colorMagenta{
	.r = 255,
	.b = 255,
};

const Color colorWhite{
	.r = 255,
	.g = 255,
	.b = 255
};

Color colorGray(const byte gray, const byte a = 255);

Color colorRandom(const byte from = 0, const byte to = 255);

int colorToInt(const Color color);
Color intToColor(const int val);

const Color depthMinValue = intToColor(std::numeric_limits<int>::min());

Color normalizedCoordToColor(const Coord3d& coord);
Color normalToColor(const Coord3d& normal);
}
