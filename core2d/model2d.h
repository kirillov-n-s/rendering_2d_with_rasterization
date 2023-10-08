#pragma once

#include "affine2d.h"

namespace Core2d {

class Model2d
{
public:
	Model2d(
		const std::vector<glm::vec3> &vertices,
		const std::vector<std::vector<char>> &adjacency);

	std::vector<glm::vec3> vertices() const;
	std::vector<std::vector<char>> adjacency() const;

	void applyTransform(const Affine2d::Mat &transform);

private:
	std::vector<glm::vec3> m_verticesInit;
	std::vector<glm::vec3> m_verticesCurrent;
	Affine2d::Mat m_transform = Affine2d::identity();

	std::vector<std::vector<char>> m_adjacency;
};
}
