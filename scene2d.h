#pragma once

#include "rendering/rendering.h"
#include "rasterization/bitmap.h"

class Scene2d
{
public:
	Scene2d(
		const int width, const int height,
		const Rasterization::Color bgColor = Rasterization::colorBlack
	);
	~Scene2d();

	void run();

private:
	static void resizeCallback(
		GLFWwindow* window,
		const int newWidth, const int newHeight);

	GLFWwindow *m_window = nullptr;
	Rendering::FramebufferData m_bufData;
	Rasterization::Bitmap m_bitmap;

	int m_width = 0;
	int m_height = 0;

	Rasterization::Color m_bgColor;
};
