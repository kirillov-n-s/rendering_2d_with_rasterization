#include <string>
#include "rasterization/bresenham.h"
#include "scene2d.h"

Scene2d::Scene2d(
    const int width, const int height,
    const Rasterization::Color bgColor)
	: m_width(width), m_height(height), m_bgColor(bgColor),
      m_bitmap(width, height, bgColor)
{
    m_window = Rendering::initWindow(width, height);
    glfwSetFramebufferSizeCallback(m_window, resizeCallback);
    glfwSetWindowUserPointer(m_window, this);
    m_bufData = Rendering::initBuffers(width, height);
}

Scene2d::~Scene2d()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Scene2d::run()
{
    float currentFrameElapsed;
    float prevFrameElapsed = 0.0f;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(m_window))
    {
        currentFrameElapsed = glfwGetTime();
        float frameTime = currentFrameElapsed - prevFrameElapsed;
        prevFrameElapsed = currentFrameElapsed;

        glfwSetWindowTitle(
            m_window,
            (std::to_string((int)(1.f / frameTime)) + " fps").c_str());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float sinTime = std::sin(currentFrameElapsed);
        const float cosTime = std::cos(currentFrameElapsed);

        const int xc = 960;
        const int yc = 540;

        const int x1 = cosTime * (640 - xc) - sinTime * (360 - yc);
        const int y1 = sinTime * (640 - xc) + cosTime * (360 - yc);
        const int x2 = cosTime * (960 - xc) - sinTime * (720 - yc);
        const int y2 = sinTime * (960 - xc) + cosTime * (720 - yc);

        const float dx = cosTime;
        const float dy = sinTime;

        const int lineWidth = 640;
        m_bitmap.clear(m_bgColor);
        const Rasterization::Color fgColor = Rasterization::colorCyan;
        for (int lineInd = 0; lineInd < lineWidth; ++lineInd) {
            const int x1Line = x1 + lineInd * dx + xc;
            const int y1Line = y1 + lineInd * dy + yc;
            const int x2Line = x2 /*+ lineInd * dx*/ + xc;
            const int y2Line = y2 /*+ lineInd * dy*/ + yc;
            Rasterization::bresenhamLine(
                glm::ivec2{ x1Line, y1Line },
                glm::ivec2{ x2Line, y2Line },
                fgColor,
                m_bitmap
            );
        }

        Rendering::sendImageToFramebuffer(
            m_bitmap.width(), m_bitmap.height(),
            m_width, m_height,
            m_bufData, m_bitmap.data()
        );

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Scene2d::resizeCallback(
    GLFWwindow* window,
    const int newWidth, const int newHeight)
{
    auto* userPtr = (Scene2d*)glfwGetWindowUserPointer(window);
    glViewport(
        0,
        0,
        userPtr->m_width = newWidth,
        userPtr->m_height = newHeight);
}
