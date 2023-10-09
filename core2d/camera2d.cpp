#include "camera2d.h"

namespace Core2d {

Camera2d::Camera2d(
	const int screenWidth, const int screenHeight,
	const float worldL, const float worldR,
	const float worldB, const float worldT)
	: m_screenWidth(screenWidth), m_screenHeight(screenHeight),
	  m_extentsInit{worldL, worldR, worldB, worldT}
{
	m_extents = m_extentsInit;
}

void Camera2d::move(const Coord2d &screenDelta)
{
	const float worldW = worldWidth();
	const float worldH = worldHeight();

	const float screenDx = screenDelta.x / m_screenWidth;
	const float screenDy = screenDelta.y / m_screenHeight;

	m_extents.worldL = m_extents.worldL - worldW * screenDx;
	m_extents.worldR = m_extents.worldR - worldW * screenDx;
	m_extents.worldB = m_extents.worldB + worldH * screenDy;
	m_extents.worldT = m_extents.worldT + worldH * screenDy;
}

void Camera2d::zoom(const HomogCoord2d& worldFixedPoint, const float factor)
{
	const float fixedX = worldFixedPoint.x;
	const float fixedY = worldFixedPoint.y;

	m_extents.worldL = fixedX - (fixedX - m_extents.worldL) / factor;
	m_extents.worldR = fixedX + (m_extents.worldR - fixedX) / factor;
	m_extents.worldB = fixedY - (fixedY - m_extents.worldB) / factor;
	m_extents.worldT = fixedY + (m_extents.worldT - fixedY) / factor;
}

void Camera2d::resizeKeepTopBottom(const int newScreenWidth, const int newScreenHeight)
{
	const float oldAspectRatio = (float)m_screenWidth / m_screenHeight;
	m_screenWidth = newScreenWidth;
	m_screenHeight = newScreenHeight;
	const float newAspectRatio = (float)m_screenWidth / m_screenHeight;
	const float aspectRatioRatio = newAspectRatio / oldAspectRatio;

	const float worldL = m_extents.worldL;
	const float worldR = m_extents.worldR;
	const float worldW = worldWidth();

	m_extents.worldL = (worldL + worldR - worldW * aspectRatioRatio) / 2.0f;
	m_extents.worldR = (worldL + worldR + worldW * aspectRatioRatio) / 2.0f;

	m_extentsInit = m_extents;
}

void Camera2d::reset()
{
	m_extents = m_extentsInit;
}

Coord2d Camera2d::worldToScreen(
	const HomogCoord2d& worldVertex) const
{
	const float worldX = worldVertex.x; /// worldVertex.z;
	const float worldY = worldVertex.y; /// worldVertex.z;
	const float screenX = (worldX - m_extents.worldL) / worldWidth() * m_screenWidth;
	const float screenY = (m_extents.worldT - worldY) / worldHeight() * m_screenHeight;
	return Coord2d(std::trunc(screenX), std::trunc(screenY));
}

HomogCoord2d Camera2d::screenToWorld(
	const Coord2d& screenVertex) const
{
	const float screenX = screenVertex.x;
	const float screenY = screenVertex.y;
	const float worldX = m_extents.worldL + worldWidth() * (screenX + 0.5) / m_screenWidth;
	const float worldY = m_extents.worldT - worldHeight() * (screenY + 0.5) / m_screenHeight;
	return HomogCoord2d(worldX, worldY, 1.0f);
}

Coords2d Camera2d::worldToScreen(
	const HomogCoords2d& worldVertices) const
{
	Coords2d screenVertices;
	screenVertices.reserve(worldVertices.size());
	for (const HomogCoord2d& worldVertex : worldVertices)
		screenVertices.push_back(worldToScreen(worldVertex));
	return screenVertices;
}

HomogCoords2d Camera2d::screenToWorld(
	const Coords2d& screenVertices) const
{
	HomogCoords2d worldVertices;
	worldVertices.reserve(worldVertices.size());
	for (const Coord2d& screenVertex : screenVertices)
		worldVertices.push_back(screenToWorld(screenVertex));
	return worldVertices;
}

float Camera2d::worldWidth() const
{
	return m_extents.worldR - m_extents.worldL;
}

float Camera2d::worldHeight() const
{
	return m_extents.worldT - m_extents.worldB;
}
}
