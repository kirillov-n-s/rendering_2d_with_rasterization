#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using HomogCoord2d = glm::vec3;
using Coord2d = glm::vec2;
using PixelCoord2d = glm::ivec2;

using HomogCoords2d = std::vector<glm::vec3>;
using Coords2d = std::vector<glm::vec2>;
using PixelCoords2d = std::vector<glm::ivec2>;

using AdjacencyVec = std::vector<bool>;
using AdjacencyMat = std::vector<AdjacencyVec>;

inline HomogCoord2d homog2d(const float x, const float y)
{
	return HomogCoord2d(x, y, 1.0f);
}

inline HomogCoord2d homog2d(const Coord2d &coord)
{
	return HomogCoord2d(coord.x, coord.y, 1.0f);
}
