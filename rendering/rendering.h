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

FramebufferData initBuffers(const int width, const int height);

void sendImageToFramebuffer(
	const int texWidth, const int texHeight,
	const int bufWidth, const int bufHeight,
	const FramebufferData bufData,
	const void *pixels);
}
