#include <cmath>
#include <glm/trigonometric.hpp>
#include "camera3d.h"

namespace Core3d {

Camera3d::Camera3d(
	const Coord3d& position,
	const float zNear, const float zFar,
	const int screenWidth, const int screenHeight,
	const float worldL, const float worldR,
	const float worldB, const float worldT)
	: m_position(position),
	  m_zNear(zNear), m_zFar(zFar),
	  m_screenWidth(screenWidth), m_screenHeight(screenHeight),
	  m_worldL(worldL), m_worldR(worldR), m_worldB(worldB), m_worldT(worldT)
{
	updateLocalSpace();
}

Coord3d Camera3d::position() const
{
	return m_position;
}

Coord3d Camera3d::front() const
{
	return m_front;
}

Coord3d Camera3d::right() const
{
	return m_right;
}

Coord3d Camera3d::up() const
{
	return m_up;
}

int Camera3d::screenWidth() const
{
	return m_screenWidth;
}

int Camera3d::screenHeight() const
{
	return m_screenHeight;
}

void Camera3d::move(const Coord3d& offset)
{
	m_position += offset;
}

void Camera3d::rotate(const float yawOffset, const float pitchOffset)
{
	m_yaw += yawOffset;
	m_pitch = glm::clamp(m_pitch + pitchOffset, -89.0f, 89.0f);
	updateLocalSpace();
}

void Camera3d::resizeKeepTopBottom(const int newScreenWidth, const int newScreenHeight)
{
	const float oldAspectRatio = (float)m_screenWidth / m_screenHeight;
	const float newAspectRatio = (float)newScreenWidth / newScreenHeight;
	const float aspectRatioRatio = newAspectRatio / oldAspectRatio;
	const float worldW = worldWidth();

	m_screenWidth = newScreenWidth;
	m_screenHeight = newScreenHeight;
	m_worldL = (m_worldL + m_worldR - worldW * aspectRatioRatio) / 2.0f;
	m_worldR = (m_worldL + m_worldR + worldW * aspectRatioRatio) / 2.0f;
}

Affine3d::Mat Camera3d::viewProjectionMat() const
{
	return projection() * view();
}

Coords2d Camera3d::worldToScreen(
	const HomogCoords3d& worldVertices,
	std::vector<bool>& outIsVertexVisible)
{
	const HomogCoords3d& clipSpaceVertices = Core3d::Affine3d::transformVertices(
		viewProjectionMat(),
		worldVertices);
	const int nVertices = clipSpaceVertices.size();
	Coords2d screenVertices;
	screenVertices.reserve(nVertices);
	outIsVertexVisible.clear();
	outIsVertexVisible.reserve(nVertices);
	for (const HomogCoord3d& clipSpaceVertex : clipSpaceVertices) {
		const float invW = 1.0f / clipSpaceVertex.w;
		const float ndcX = clipSpaceVertex.x * invW;
		const float ndcY = clipSpaceVertex.y * invW;
		const float ndcZ = clipSpaceVertex.z * invW;
		const float screenX = (ndcX + 1.0f) * 0.5f * m_screenWidth;
		const float screenY = (1.0f - ndcY) * 0.5f * m_screenHeight;
		screenVertices.emplace_back(std::trunc(screenX), std::trunc(screenY));
		outIsVertexVisible.push_back(ndcZ < 1.0f);
	}
	return screenVertices;
}

Coords3d Camera3d::worldToScreenWithDepth(const HomogCoords3d& worldVertices)
{
	const HomogCoords3d& clipSpaceVertices = Core3d::Affine3d::transformVertices(
		viewProjectionMat(),
		worldVertices);
	const int nVertices = clipSpaceVertices.size();
	Coords3d screenVerticesWithDepth;
	screenVerticesWithDepth.reserve(nVertices);
	for (const HomogCoord3d& clipSpaceVertex : clipSpaceVertices) {
		const float invW = 1.0f / clipSpaceVertex.w;
		const float ndcX = clipSpaceVertex.x * invW;
		const float ndcY = clipSpaceVertex.y * invW;
		const float ndcZ = clipSpaceVertex.z * invW;
		const float screenX = (ndcX + 1.0f) * 0.5f * m_screenWidth;
		const float screenY = (1.0f - ndcY) * 0.5f * m_screenHeight;
		const float screenZ = -ndcZ * worldDepth();
		screenVerticesWithDepth.emplace_back(
			std::trunc(screenX), std::trunc(screenY), std::trunc(screenZ)
		);
	}
	return screenVerticesWithDepth;
}

float Camera3d::worldWidth() const
{
	return m_worldR - m_worldL;
}

float Camera3d::worldHeight() const
{
	return m_worldT - m_worldB;
}

float Camera3d::worldDepth() const
{
	return m_zFar - m_zNear;
}

Affine3d::Mat Camera3d::view() const
{
	const Coord3d kV = m_front;
	const Coord3d iV = m_right;
	const Coord3d jV = m_up;

	Affine3d::Mat mat = Affine3d::identity();
	mat[0][0] = iV.x;
	mat[1][0] = iV.y;
	mat[2][0] = iV.z;
	mat[0][1] = jV.x;
	mat[1][1] = jV.y;
	mat[2][1] = jV.z;
	mat[0][2] = -kV.x;
	mat[1][2] = -kV.y;
	mat[2][2] = -kV.z;
	mat[3][0] = -glm::dot(iV, m_position);
	mat[3][1] = -glm::dot(jV, m_position);
	mat[3][2] = glm::dot(kV, m_position);

	return mat;
}

Affine3d::Mat Camera3d::projection() const
{
	const float invWorldW = 1.0f / worldWidth();
	const float invWorldH = 1.0f / worldHeight();
	const float invWorldD = 1.0f / worldDepth();

	Affine3d::Mat mat = Affine3d::identity();
	mat[0][0] = 2.0f * m_zNear * invWorldW;
	mat[1][1] = 2.0f * m_zNear * invWorldH;
	mat[2][0] = (m_worldR + m_worldL) * invWorldW;
	mat[2][1] = (m_worldT + m_worldB) * invWorldH;
	mat[2][2] = -(m_zFar + m_zNear) * invWorldD;
	mat[2][3] = -1.0f;
	mat[3][2] = -2.0f * m_zFar * m_zNear * invWorldD;
	mat[3][3] = 0.0f;

	return mat;
}

void Camera3d::updateLocalSpace()
{
	const float radYaw = glm::radians(m_yaw);
	const float radPitch = glm::radians(m_pitch);

	const float cosYaw = std::cos(radYaw);
	const float cosPitch = std::cos(radPitch);
	const float sinYaw = std::sin(radYaw);
	const float sinPitch = std::sin(radPitch);

	const Coord3d front(cosYaw * cosPitch, sinPitch, sinYaw * cosPitch);

	m_front = glm::normalize(front);
	m_right = glm::normalize(glm::cross(m_front, Directions3d::up));
	m_up = glm::cross(m_right, m_front);
}
}
