// 2. масштаб с сохр верхней и нижней границ

#pragma once

#include "../rasterization/segment.h"
#include "model2d.h"

namespace Core2d {

class Camera2d
{
public:
	Camera2d(
		const int screenWidth, const int screenHeight,
		const float worldL, const float worldR,
		const float worldB, const float worldT,
		const glm::vec2 &worldPos = glm::vec2(0.0f));

	void moveTo(const glm::vec2 newWorldPos);
	void moveBy(const glm::vec2 worldVec);

	std::vector<Rasterization::Segment> worldToScreen(
		const Model2d& model);

private:
	int m_screenWidth;
	int m_screenHeight;

	float m_worldL;
	float m_worldR;
	float m_worldB;
	float m_worldT;
	
	glm::vec2 m_worldPos;
};
}
