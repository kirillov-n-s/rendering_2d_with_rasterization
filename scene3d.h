#pragma once

#include "rendering/rendering.h"
#include "rasterization/bitmap.h"
#include "core3d/model3dwireandpoly.h"
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

	void setAxisModels(
		const std::array<Core3d::Model3dWireframe, 3> &models,
		const std::array<Rasterization::Color, 3> &colors);

	void addModel(
		const Core3d::Model3dWireAndPoly& model,
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

	std::vector<Core3d::Model3dWireframe> m_axisModels;
	std::vector<Rasterization::Color> m_axisColors;

	std::vector<Core3d::Model3dWireAndPoly> m_models;
	std::vector<Rasterization::Color> m_modelColors;

	Core3d::Camera3d m_camera;

	float m_movementSpeed = 0.75f;
	float m_rotationSpeed = 0.2f;
	bool m_camRotationOn = false;
	bool m_camTranslationOn = true;

	Coord3d m_pivotTranslation{ 0.0f };
	Coord3d m_pivotYawPitchRoll{ 0.0f };
	bool m_pivotTranslationOn = false;

	bool m_modelOrPivotRotationOn = false;
	bool m_modelXScalingOn = false;
	bool m_modelYScalingOn = false;
	bool m_modelZScalingOn = false;

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
