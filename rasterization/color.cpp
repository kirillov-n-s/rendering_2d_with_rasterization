#include "color.h"

namespace Rasterization {

Color colorGray(const byte gray, const byte a)
{
	return Color{
		.r = gray,
		.g = gray,
		.b = gray,
		.a = a
	};
}

Color colorRandom(const byte from, const byte to)
{
	std::default_random_engine randomEngine(std::random_device{}());
	std::uniform_int_distribution<int> distribution(from, to);
	return Color{
		.r = byte(distribution(randomEngine)),
		.g = byte(distribution(randomEngine)),
		.b = byte(distribution(randomEngine))
	};
}

int colorToInt(const Color color)
{
	return *reinterpret_cast<const int*>(&color);
}

Color intToColor(const int val)
{
	return *reinterpret_cast<const Color*>(&val);
}

Color normalizedCoordToColor(const Coord3d& coord)
{
	return Color{
		.r = byte(coord.x * 255.0f),
		.g = byte(coord.y * 255.0f),
		.b = byte(coord.z * 255.0f),
	};
}

Color normalToColor(const Coord3d& normal)
{
	const Coord3d normal01 = (normal + 1.0f) * 0.5f;
	return normalizedCoordToColor(normal01);
}
}
