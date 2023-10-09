#pragma once

#include <iostream>
#include <string>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

namespace Rendering {

struct FramebufferData
{
	GLuint readFbo;
	GLuint colorAttachment;
};

GLFWwindow *initWindow(const int width, const int height);

FramebufferData initFramebuffer();

void sendImageToFramebuffer(
	const int width, const int height,
	const FramebufferData bufData,
	const void *pixels);
}
