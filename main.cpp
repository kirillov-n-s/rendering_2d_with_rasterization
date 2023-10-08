#include "scene2d.h"

int main()
{
	Scene2d scene(
		1920, 1080,
		-1, 1, -1, 1,
		Rasterization::colorGray(100)
	);

	const HomogCoords2d arrowXVertices{
		homog2d(-1, 0), homog2d(1, 0), homog2d(0.9, 0.03), homog2d(0.9, -0.03)
	};
	const HomogCoords2d arrowYVertices{
		homog2d(0, -1), homog2d(0, 1), homog2d(0.015, 0.9), homog2d(-0.015, 0.9)
	};
	const AdjacencyMat arrowAdjacency{
		{0, 1, 0, 0},
		{1, 0, 1, 1},
		{0, 1, 0, 0},
		{0, 1, 0, 0}
	};

	const HomogCoords2d houseVertices{
		homog2d(0.2, 0.2), homog2d(0.6, 0.2), homog2d(0.6, 0.6),
		homog2d(0.4, 0.8), homog2d(0.2, 0.6),
		homog2d(0.6 - 0.4 / 3, 0.8 - 0.2 / 3),
		homog2d(0.6 - 0.2 / 3, 0.8 - 0.2 / 3), homog2d(0.6 - 0.2 / 3, 0.8 - 0.4 / 3)
	};
	const AdjacencyMat houseAdjacency{
		{0, 1, 0, 0, 1, 0, 0, 0},
		{1, 0, 1, 0, 0, 0, 0, 0},
		{0, 1, 0, 1, 1, 0, 0, 0},
		{0, 0, 1, 0, 1, 0, 0, 0},
		{1, 0, 1, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0, 1, 0, 1},
		{0, 0, 0, 0, 0, 0, 1, 0},
	};

	scene.addModel(
		Core2d::Model2d(arrowXVertices, arrowAdjacency),
		Rasterization::colorBlue);
	scene.addModel(
		Core2d::Model2d(arrowYVertices, arrowAdjacency),
		Rasterization::colorRed);

	scene.addModel(
		Core2d::Model2d(houseVertices, houseAdjacency),
		Rasterization::colorCyan);

	scene.run();
}
