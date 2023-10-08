#include "camera2d.h"

namespace Core2d {

Camera2d::Camera2d(
	const int screenWidth, const int screenHeight,
	const float worldL, const float worldR,
	const float worldB, const float worldT,
	const glm::vec2& worldPos = glm::vec2(0.0f))
	: m_screenWidth(screenWidth), m_screenHeight(screenHeight),
	  m_worldL(worldL), m_worldR(worldR), m_worldB(worldB), m_worldT(worldT)
{}

void Camera2d::moveTo(const glm::vec2 newWorldPos)
{
	m_worldPos = newWorldPos;
}

void Camera2d::moveBy(const glm::vec2 worldVec)
{
	m_worldPos += worldVec;
}

std::vector<Rasterization::Segment> Camera2d::worldToScreen(
	const Model2d& model)
{

}
}
