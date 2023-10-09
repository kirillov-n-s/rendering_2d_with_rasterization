#pragma once

#include <optional>
#include "rendering/rendering.h"
#include "rasterization/bitmap.h"
#include "core2d/model2d.h"
#include "core2d/camera2d.h"

class Scene2d
{
public:
	Scene2d(
		const int screenWidth, const int screenHeight,
		const float worldL, const float worldR,
		const float worldB, const float worldT,
		const Rasterization::Color bgColor = Rasterization::colorBlack
	);
	~Scene2d();

	Core2d::Camera2d camera() const;

	void addModel(
		const Core2d::Model2d& model,
		const Rasterization::Color &color,
		const bool isSelected = false);

	void run();

private:
	void setTitle(const float frameTime);
	void render();
	void handleTransform(const float frameTime);

	GLFWwindow *m_window = nullptr;
	Rendering::FramebufferData m_bufData;

	int m_width = 0;
	int m_height = 0;

	Coord2d m_cursorPos;
	bool m_isPressed = false;

	Rasterization::Color m_bgColor;
	std::vector<Rasterization::Color> m_modelColors;

	std::vector<Core2d::Model2d> m_models;
	Core2d::Camera2d m_camera;

	HomogCoords2d m_verticesInProgress;
	AdjacencyMat m_adjacencyInProgress;

	std::optional<int> m_selectedModelInd;
	Rasterization::Color m_selectionColor = Rasterization::colorWhite;

	float m_zoomFactor = 0.1f;

	float m_translationParam = 0.01f;
	float m_rotationParam = 0.01f;
	float m_scaleParam = 0.01f;

	static void resizeCallback(
		GLFWwindow* window,
		const int newWidth, const int newHeight);

	static void cursorPosCallback(
		GLFWwindow* window,
		const double newX, const double newY);

	static void scrollCallback(
		GLFWwindow* window,
		const double dx, const double dy);

	static void mouseButtonCallback(
		GLFWwindow* window,
		const int button, const int action, const int mods);

	static void keyCallback(
		GLFWwindow* window,
		const int key, const int scancode, const int action, const int mods);
};
