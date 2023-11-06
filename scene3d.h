#pragma once

#include "rendering/rendering.h"
#include "rasterization/bitmap.h"
#include "core3d/model3d.h"
#include "core3d/camera3d.h"

class Scene3d
{
public:
	Scene3d(
		const Core3d::Camera3d& camera,
		const int pixelSize = 1,
		const Rasterization::Color bgColor = Rasterization::colorBlack
	);
	~Scene3d();

	void addModel(
		const Core3d::Model3d& model,
		const Rasterization::Color& color);

	void run();

private:
	void setTitle(const float frameTime);
	void render();
	void handleTransform(const float frameTime);

	GLFWwindow* m_window = nullptr;
	Rendering::FramebufferData m_bufData;

	int m_width = 0;
	int m_height = 0;
	int m_pixelSize = 1;

	Coord2d m_cursorPos;

	Rasterization::Color m_bgColor;
	std::vector<Rasterization::Color> m_modelColors;

	std::vector<Core3d::Model3d> m_models;
	Core3d::Camera3d m_camera;

	bool m_camRotationOn = false;

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