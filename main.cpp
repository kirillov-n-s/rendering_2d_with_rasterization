#include <array>
#include <cmath>
//#include "scene2d.h"
#include "scene3d.h"
#include "io/obj.h"

/*int main2d()
{
	Core2d::Camera2d camera(300, 300, -1, 1, -1, 1);

	const HomogCoords2d axesVertices{
		homog2d(-1, 0), homog2d(1, 0), homog2d(0.9, 0.03), homog2d(0.9, -0.03),
		homog2d(0, -1), homog2d(0, 1), homog2d(0.015, 0.9), homog2d(-0.015, 0.9)
	};
	const AdjacencyMat axesAdjacency{
		{0, 1, 0, 0, 0, 0, 0, 0},
		{1, 0, 1, 1, 0, 0, 0, 0},
		{0, 1, 0, 0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 0, 1, 0, 1, 1},
		{0, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0, 1, 0, 0}
	};

	const HomogCoords2d houseVertices{
		homog2d(-0.2, 0.2), homog2d(-0.6, 0.2), homog2d(-0.6, 0.6),
		homog2d(-0.4, 0.8), homog2d(-0.2, 0.6),
		homog2d(-0.6 + 0.4 / 3, 0.8 - 0.2 / 3),
		homog2d(-0.6 + 0.2 / 3, 0.8 - 0.2 / 3), homog2d(-0.6 + 0.2 / 3, 0.8 - 0.4 / 3)
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

	const HomogCoords2d plotVertices = Core2d::makeVerticesFromFunction(
		&sinf,
		-3.14f, 3.14f, 0.01f,
		1.0f / 3.14f);
	const int nPlotVertices = plotVertices.size();
	const AdjacencyMat plotAdjacency = Core2d::makePolylineAdjacency(nPlotVertices);

	Scene2d scene(camera, 3, Rasterization::colorBlack);

	scene.addModel(
		Core2d::Model2d(axesVertices, axesAdjacency),
		Rasterization::colorRed);
	scene.setAxesModel(0);

	scene.addModel(
		Core2d::Model2d(axesVertices, axesAdjacency),
		Rasterization::colorBlue);

	scene.addModel(
		Core2d::Model2d(houseVertices, houseAdjacency),
		Rasterization::colorCyan);
	scene.addModel(
		Core2d::Model2d(plotVertices, plotAdjacency),
		Rasterization::colorCyan);

	scene.run();
}*/

int main()
{
	Core3d::Camera3d camera(
		(Directions3d::forward + Directions3d::up * 0.5f) * 30.0f,
		100.0f, 1000.0f,
		300, 300,
		-100.0f, 100.0f, -100.0f, 100.0f
	);
	camera.rotate(0.0f, -30.0f);

	HomogCoords3d xAxisVertices{
		homog3d(Directions3d::zero),
		homog3d(Directions3d::right * 10.0f),
	};
	HomogCoords3d yAxisVertices{
		homog3d(Directions3d::zero),
		homog3d(Directions3d::up * 10.0f),
	};
	HomogCoords3d zAxisVertices{
		homog3d(Directions3d::zero),
		homog3d(Directions3d::forward * 10.0f),
	};
	AdjacencyMat lineAdjacency{ {0, 1}, {1, 0} };

	Core3d::Model3dWireframe xAxisModel(xAxisVertices, lineAdjacency);
	Core3d::Model3dWireframe yAxisModel(yAxisVertices, lineAdjacency);
	Core3d::Model3dWireframe zAxisModel(zAxisVertices, lineAdjacency);

	HomogCoords3d objVertices;
	IndexVec objTriangleVertexIndices;
	const std::string error = IO::verticesAndTrianglesFromObj(
		//R"(C:\Users\kirillov_n_s\Downloads\Toilet_V2_L3.123cb6db0b23-9e9b-42d2-958e-438868919b76\10778_Toilet_V2.obj)",
		R"(C:\Users\kirillov_n_s\Desktop\University\Sem1\CG\laba1\assets\Skull_50k.obj)",
		objVertices,
		objTriangleVertexIndices);
	if (!error.empty()) {
		std::cerr << error;
		exit(1);
	}
	std::cout
		<< "nVertices = " << objVertices.size() << "\n"
		<< "nTriangles = " << objTriangleVertexIndices.size() / 3 << std::endl;
	Core3d::Model3dPoly objModel(objVertices, objTriangleVertexIndices);

	Scene3d scene(camera, 3, Rasterization::colorGray(64));

	scene.setAxisModels(
		{ xAxisModel, yAxisModel, zAxisModel },
		{ Rasterization::colorRed, Rasterization::colorGreen, Rasterization::colorBlue });
	scene.addModel(objModel, {});

	scene.run();
}
