#pragma once

#include <random>

using byte = unsigned char;

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

inline Color colorGray(const byte gray, const byte a = 255)
{
	return Color{
		.r = gray,
		.g = gray,
		.b = gray,
		.a = a
	};
}

inline Color colorRandom(const byte from = 0, const byte to = 255)
{
	std::default_random_engine randomEngine(std::random_device{}());
	std::uniform_int_distribution<int> distribution(from, to);
	return Color{
		.r = byte(distribution(randomEngine)),
		.g = byte(distribution(randomEngine)),
		.b = byte(distribution(randomEngine))
	};
}

inline int colorToInt(const Color color)
{
	return *reinterpret_cast<const int *>(&color);
}

inline Color intToColor(const int val)
{
	return *reinterpret_cast<const Color *>(&val);
}

const Color depthMinValue = intToColor(std::numeric_limits<int>::min());
}
