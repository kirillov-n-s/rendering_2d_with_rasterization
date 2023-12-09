#pragma once

#include <functional>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using HomogCoord2d = glm::vec3;
using Coord2d = glm::vec2;
using PixelCoord2d = glm::ivec2;
using PixelCoord3d = glm::ivec3;

using HomogCoords2d = std::vector<glm::vec3>;
using Coords2d = std::vector<glm::vec2>;
using Coords3d = std::vector<glm::vec3>;
using PixelCoords2d = std::vector<glm::ivec2>;
using PixelCoords3d = std::vector<glm::ivec3>;

using AdjacencyVec = std::vector<bool>;
using AdjacencyMat = std::vector<AdjacencyVec>;
using IndexVec = std::vector<int>;

inline HomogCoord2d homog2d(const float x, const float y)
{
	return HomogCoord2d(x, y, 1.0f);
}

inline HomogCoord2d homog2d(const Coord2d &coord)
{
	return HomogCoord2d(coord.x, coord.y, 1.0f);
}

using HomogCoord3d = glm::vec4;
using Coord3d = glm::vec3;

using HomogCoords3d = std::vector<glm::vec4>;

inline HomogCoord3d homog3d(const float x, const float y, const float z)
{
	return HomogCoord3d(x, y, z, 1.0f);
}

inline HomogCoord3d homog3d(const Coord3d coord)
{
	return HomogCoord3d(coord.x, coord.y, coord.z, 1.0f);
}

AdjacencyMat makeZeroAdjacency(const int nVertices);

AdjacencyMat modifyAdjacency(
	const AdjacencyMat& adjacency,
	const AdjacencyVec& modVec,
	const std::function<bool(bool, bool)>& reduceFn);

namespace Directions3d {

const Coord3d zero(0.0f, 0.0f, 0.0f);
const Coord3d one(1.0f, 1.0f, 1.0f);

const Coord3d up(0.0f, 1.0f, 0.0f);
const Coord3d down(0.0f, -1.0f, 0.0f);

const Coord3d right(1.0f, 0.0f, 0.0f);
const Coord3d left(-1.0f, 0.0f, 0.0f);

const Coord3d forward(0.0f, 0.0f, 1.0f);
const Coord3d backward(0.0f, 0.0f, -1.0f);
}
