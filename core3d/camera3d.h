#pragma once

#include "../common_types.h"
#include "affine3d.h"

namespace Core3d {

class Camera3d {
public:
	Camera3d(
		const Coord3d& position,
		const float zNear, const float zFar,
		const int screenWidth, const int screenHeight,
		const float worldL, const float worldR,
		const float worldB, const float worldT);

	Coord3d position() const;
	Coord3d front() const;
	Coord3d right() const;
	Coord3d up() const;

	int screenWidth() const;
	int screenHeight() const;

	void move(const Coord3d& offset);
	void rotate(const float yawOffset, const float pitchOffset);
	void resizeKeepTopBottom(const int newScreenWidth, const int newScreenHeight);

	Affine3d::Mat viewProjectionMat() const;

	Coords2d worldToScreen(
		const HomogCoords3d& worldVertices,
		std::vector<bool> &outIsVertexVisible);

	Coords3d worldToScreenWithDepth(const HomogCoords3d& worldVertices);

private:
	float worldWidth() const;
	float worldHeight() const;
	float worldDepth() const;

	Affine3d::Mat view() const;
	Affine3d::Mat projection() const;

	void updateLocalSpace();

	Coord3d m_position;
	Coord3d m_front;
	Coord3d m_right;
	Coord3d m_up;

	int m_screenWidth = 0;
	int m_screenHeight = 0;

	float m_zNear = 0.0f;
	float m_zFar = 0.0f;

	float m_worldL = 0.0f;
	float m_worldR = 0.0f;
	float m_worldB = 0.0f;
	float m_worldT = 0.0f;

	float m_yaw = -90.0f;
	float m_pitch = 0.0f;
};
}
