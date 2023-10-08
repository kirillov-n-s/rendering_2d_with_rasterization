#include "rendering.h"

namespace Rendering {

GLFWwindow *initWindow(const int width, const int height)
{
    glfwInit();
    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MAJOR,
        3);
    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MINOR,
        3);
    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        width,
        height,
        "",
        nullptr,
        nullptr);
    if (window == nullptr) {
        glfwTerminate();
        std::cerr << "Cannot open window";
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(
        window,
        GLFW_CURSOR,
        GLFW_CURSOR_NORMAL);

    glViewport(
        0,
        0,
        width,
        height);

    glewInit();

    return window;
}

FramebufferData initBuffers(const int width, const int height)
{
    FramebufferData bufData;

    glGenTextures(1, &bufData.colorAttachment);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bufData.colorAttachment);
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width, height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenFramebuffers(1, &bufData.readFbo);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, bufData.readFbo);
    glFramebufferTexture2D(
        GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        bufData.colorAttachment, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer not complete: status " << status;
        exit(-1);
    }

    return bufData;
}

void sendImageToFramebuffer(
    const int texWidth, const int texHeight,
    const int bufWidth, const int bufHeight,
    const FramebufferData bufData,
    const void *pixels)
{
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0, 0, 0, texWidth, texHeight,
        GL_RGBA, GL_UNSIGNED_BYTE,
        pixels);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, bufData.readFbo);
    glBlitFramebuffer(
        0, 0, texWidth, texHeight,
        0, 0, bufWidth, bufHeight,
        GL_COLOR_BUFFER_BIT,
        GL_LINEAR);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}
}
