#include <string>
#include "rasterization/rasterization.h"
#include "scene2d.h"

Scene2d::Scene2d(
    const int screenWidth, const int screenHeight,
    const float worldL, const float worldR,
    const float worldB, const float worldT,
    const Rasterization::Color bgColor)
	: m_width(screenWidth), m_height(screenHeight), m_bgColor(bgColor),
      m_camera(
          screenWidth, screenHeight,
          worldL, worldR, worldB, worldT)
{
    m_window = Rendering::initWindow(screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(m_window, resizeCallback);
    glfwSetCursorPosCallback(m_window, cursorPosCallback);
    glfwSetScrollCallback(m_window, scrollCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetWindowUserPointer(m_window, this);
    m_bufData = Rendering::initBuffers(screenWidth, screenHeight);
}

Scene2d::~Scene2d()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

Core2d::Camera2d Scene2d::camera() const
{
    return m_camera;
}

void Scene2d::addModel(
    const Core2d::Model2d& model,
    const Rasterization::Color& color)
{
    m_models.push_back(model);
    m_modelColors.push_back(color);
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

        renderState();

        std::cout << "Cursor (screen) = " << m_cursorPos.x << ' ' << m_cursorPos.y << std::endl;
        const HomogCoord2d worldCursorPos = m_camera.screenToWorld(m_cursorPos);
        std::cout << "Cursor (world) = " << worldCursorPos.x << ' ' << worldCursorPos.y << std::endl;

        glfwSwapBuffers(m_window);

        glfwPollEvents();
        if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            m_camera.reset();
        }
    }

    glfwTerminate();
}

void Scene2d::renderState()
{
    Rasterization::Bitmap bitmap(m_width, m_height, m_bgColor);
    bitmap.clear(m_bgColor);
    const int nModels = m_models.size();
    for (int modelInd = 0; modelInd < nModels; ++modelInd) {
        const Core2d::Model2d& model = m_models[modelInd];
        const Rasterization::Color color = m_modelColors[modelInd];
        const Coords2d& screenVertices = m_camera.worldToScreen(
            model.vertices());
        Rasterization::rasterizeModel(
            screenVertices,
            model.adjacency(),
            color,
            bitmap);
    }

    Rendering::sendImageToFramebuffer(
        bitmap.width(), bitmap.height(),
        m_width, m_height,
        m_bufData, bitmap.data());
}

void Scene2d::resizeCallback(
    GLFWwindow* window,
    const int newWidth, const int newHeight)
{
    auto* scenePtr = (Scene2d*)glfwGetWindowUserPointer(window);
    scenePtr->m_camera.resizeKeepTopBottom(newWidth, newHeight);
    glViewport(
        0,
        0,
        scenePtr->m_width = newWidth,
        scenePtr->m_height = newHeight);
}

void Scene2d::cursorPosCallback(
    GLFWwindow* window,
    const double newX, const double newY)
{
    auto* scenePtr = (Scene2d*)glfwGetWindowUserPointer(window);
    const Coord2d newCursorPos = Coord2d(newX, newY);
    if (scenePtr->m_isPressed) {
        const Coord2d dxy = newCursorPos - scenePtr->m_cursorPos;
        scenePtr->m_camera.move(dxy);
    }
    scenePtr->m_cursorPos = newCursorPos;
}

void Scene2d::scrollCallback(
    GLFWwindow* window,
    const double dx, const double dy)
{
    auto* scenePtr = (Scene2d*)glfwGetWindowUserPointer(window);
    scenePtr->m_camera.zoom(
        scenePtr->m_camera.screenToWorld(scenePtr->m_cursorPos),
        dy > 0 ? 1.1 : 0.9
    );
}

void Scene2d::mouseButtonCallback(
    GLFWwindow* window,
    const int button, const int action, const int mods)
{
    auto* scenePtr = (Scene2d*)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        scenePtr->m_isPressed = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        scenePtr->m_isPressed = false;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        const HomogCoord2d newVertex = scenePtr->m_camera.screenToWorld(
            scenePtr->m_cursorPos);
        scenePtr->m_verticesInProgress.push_back(newVertex);
    }
}

void Scene2d::keyCallback(
    GLFWwindow* window,
    const int key, const int scancode, const int action, const int mods)
{
    auto* scenePtr = (Scene2d*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        const int nVertices = scenePtr->m_verticesInProgress.size();
        AdjacencyMat adjacency;
        adjacency.reserve(nVertices);
        for (int vertexInd = 0; vertexInd < nVertices; ++vertexInd) {
            adjacency.emplace_back(nVertices);
            for (int adjInd = 0; adjInd < nVertices; ++adjInd) {
                const bool areVerticesFollowing = std::abs(vertexInd - adjInd) == 1;
                adjacency[vertexInd][adjInd] = areVerticesFollowing;
            }
        }
        scenePtr->m_models.emplace_back(
            scenePtr->m_verticesInProgress, adjacency);
        scenePtr->m_modelColors.push_back(Rasterization::colorMagenta);
        scenePtr->m_verticesInProgress.clear();
    }
}
