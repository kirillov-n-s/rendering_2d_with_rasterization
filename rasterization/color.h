#pragma once

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
}

