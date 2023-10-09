#include <sstream>
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
    const Rasterization::Color& color,
    const bool isSelected)
{
    m_models.push_back(model);
    m_modelColors.push_back(color);
    if (isSelected)
        m_selectedModelInd = m_models.size() - 1;
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
        
        setTitle(frameTime);

        render();
        glfwSwapBuffers(m_window);

        glfwPollEvents();
        handleTransform(frameTime);
    }

    glfwTerminate();
}

void Scene2d::setTitle(const float frameTime)
{
    const HomogCoord2d worldCursorPos = m_camera.screenToWorld(m_cursorPos);
    std::ostringstream title;
    title << "FPS: " << (int)(1.0f / frameTime) << ". ";
    title << "Cursor (screen): " << m_cursorPos.x << ' ' << m_cursorPos.y << ". ";
    title << "Cursor (world): " << worldCursorPos.x << ' ' << worldCursorPos.y;
    glfwSetWindowTitle(m_window, title.str().c_str());
}

void Scene2d::render()
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
            !m_selectedModelInd.has_value() || *m_selectedModelInd != modelInd
                ? color
                : m_selectionColor,
            bitmap);
    }

    if (m_verticesInProgress.size() > 1) {
        const Coords2d& screenVerticesInProgress = m_camera.worldToScreen(
            m_verticesInProgress);
        Rasterization::rasterizeModel(
            screenVerticesInProgress,
            m_adjacencyInProgress,
            Rasterization::colorMagenta,
            bitmap);
    }

    Rendering::sendImageToFramebuffer(
        bitmap.width(), bitmap.height(),
        m_width, m_height,
        m_bufData, bitmap.data());
}

void Scene2d::handleTransform(const float frameTime)
{
    if (!m_selectedModelInd.has_value())
        return;

    Core2d::Model2d& selectedModel = m_models[*m_selectedModelInd];
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
        const Core2d::Affine2d::Mat transform =
            Core2d::Affine2d::translation(Coord2d(0.0f, m_translationParam));
        selectedModel.applyTransform(transform);
    }
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
        const Core2d::Affine2d::Mat transform =
            Core2d::Affine2d::translation(Coord2d(-m_translationParam, 0.0f));
        selectedModel.applyTransform(transform);
    }
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
        const Core2d::Affine2d::Mat transform =
            Core2d::Affine2d::translation(Coord2d(0.0f, -m_translationParam));
        selectedModel.applyTransform(transform);
    }
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
        const Core2d::Affine2d::Mat transform =
            Core2d::Affine2d::translation(Coord2d(m_translationParam, 0.0f));
        selectedModel.applyTransform(transform);
    }

    if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) {
        const Core2d::Affine2d::Mat transform =
            Core2d::Affine2d::rotation(m_rotationParam);
        selectedModel.applyTransform(transform);
    }
    if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) {
        const Core2d::Affine2d::Mat transform =
            Core2d::Affine2d::rotation(-m_rotationParam);
        selectedModel.applyTransform(transform);
    }

    if (glfwGetKey(m_window, GLFW_KEY_Z) == GLFW_PRESS) {
        const Core2d::Affine2d::Mat transform =
            Core2d::Affine2d::scale(Coord2d(1.0f + m_scaleParam, 1.0f));
        selectedModel.applyTransform(transform);
    }
    if (glfwGetKey(m_window, GLFW_KEY_X) == GLFW_PRESS) {
        const Core2d::Affine2d::Mat transform =
            Core2d::Affine2d::scale(Coord2d(1.0f, 1.0f + m_scaleParam));
        selectedModel.applyTransform(transform);
    }
    if (glfwGetKey(m_window, GLFW_KEY_C) == GLFW_PRESS) {
        const Core2d::Affine2d::Mat transform =
            Core2d::Affine2d::scale(Coord2d(1.0f - m_scaleParam, 1.0f));
        selectedModel.applyTransform(transform);
    }
    if (glfwGetKey(m_window, GLFW_KEY_V) == GLFW_PRESS) {
        const Core2d::Affine2d::Mat transform =
            Core2d::Affine2d::scale(Coord2d(1.0f, 1.0f - m_scaleParam));
        selectedModel.applyTransform(transform);
    }

    if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
        const Core2d::Affine2d::Mat transform =
            Core2d::Affine2d::scale(Coord2d(1.0f + m_scaleParam, 1.0f + m_scaleParam));
        selectedModel.applyTransform(transform);
    }
    if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_PRESS) {
        const Core2d::Affine2d::Mat transform =
            Core2d::Affine2d::scale(Coord2d(1.0f - m_scaleParam, 1.0f - m_scaleParam));
        selectedModel.applyTransform(transform);
    }
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
        1.0f + scenePtr->m_zoomFactor * dy
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
        const int nVertices = scenePtr->m_verticesInProgress.size();
        scenePtr->m_adjacencyInProgress = Core2d::makePolylineAdjacency(nVertices);
    }
}

void Scene2d::keyCallback(
    GLFWwindow* window,
    const int key, const int scancode, const int action, const int mods)
{
    auto* scenePtr = (Scene2d*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(scenePtr->m_window, true);
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        scenePtr->m_camera.reset();
    if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE
            && scenePtr->m_verticesInProgress.size() >= 2) {
        scenePtr->m_models.push_back(
            Core2d::Model2d(
                scenePtr->m_verticesInProgress,
                scenePtr->m_adjacencyInProgress));
        scenePtr->m_modelColors.push_back(Rasterization::colorYellow);
        scenePtr->m_verticesInProgress.clear();
    }

    const bool hasSelection = scenePtr->m_selectedModelInd.has_value();
    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
        scenePtr->m_selectedModelInd = hasSelection ? std::nullopt : std::optional(0);
        return;
    }

    if (!hasSelection)
        return;
    const int nModels = scenePtr->m_models.size();
    const int selectedModelInd = *scenePtr->m_selectedModelInd;
    Core2d::Model2d& selectedModel = scenePtr->m_models[selectedModelInd];
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
        scenePtr->m_selectedModelInd = (selectedModelInd + 1) % nModels;
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
        scenePtr->m_selectedModelInd = (nModels + selectedModelInd - 1) % nModels;
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
        selectedModel.resetTransform();
}
