#include <sstream>
#include <string>
#include "rasterization/rasterization.h"
#include "scene3d.h"

Scene3d::Scene3d(
    const Core3d::Camera3d& camera,
    const int pixelSize,
    const Rasterization::Color bgColor)
    : m_width(camera.screenWidth()), m_height(camera.screenHeight()), m_pixelSize(pixelSize),
      m_bgColor(bgColor),
      m_camera(camera)
{
    m_window = Rendering::initWindow(
        m_width * pixelSize,
        m_height * pixelSize);
    glfwSetFramebufferSizeCallback(m_window, resizeCallback);
    glfwSetCursorPosCallback(m_window, cursorPosCallback);
    glfwSetScrollCallback(m_window, scrollCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetWindowUserPointer(m_window, this);
    m_bufData = Rendering::initFramebuffer();
}

Scene3d::~Scene3d()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Scene3d::addModel(
    const Core3d::Model3d& model,
    const Rasterization::Color& color)
{
    m_models.push_back(model);
    m_modelColors.push_back(color);
}

void Scene3d::run()
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

void Scene3d::setTitle(const float frameTime)
{
    const Coord3d cameraPos = m_camera.position();
    const Coord3d cameraDir = m_camera.direction();
    std::ostringstream title;
    title << "FPS: " << (int)(1.0f / frameTime) << ". ";
    title << "Cursor pos: " << m_cursorPos.x << ' ' << m_cursorPos.y << ". ";
    title << "Camera pos: " << cameraPos.x << ' ' << cameraPos.y << ' ' << cameraPos.z << ". ";
    title << "Camera dir: " << cameraDir.x << ' ' << cameraDir.y << ' ' << cameraDir.z;
    glfwSetWindowTitle(m_window, title.str().c_str());
}

void Scene3d::render()
{
    Rasterization::Bitmap bitmap(m_width, m_height, m_bgColor, m_pixelSize);
    bitmap.clear(m_bgColor);

    const int nModels = m_models.size();
    for (int modelInd = 0; modelInd < nModels; ++modelInd) {
        const Core3d::Model3d& model = m_models[modelInd];
        const Coords2d screenVertices = m_camera.worldToScreen(model.vertices());

        Rasterization::Color color = m_modelColors[modelInd];
        /*if (modelInd == m_selectedModelInd)
            color = modelInd == m_axesModelInd ? m_axesSelectionColor : m_selectionColor;*/

        Rasterization::rasterizeModel(
            screenVertices,
            model.adjacency(),
            color,
            bitmap);
    }

    Rendering::sendImageToFramebuffer(
        bitmap.width(), bitmap.height(),
        m_bufData, bitmap.data());
}

void Scene3d::handleTransform(const float frameTime)
{
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera.move(m_camera.direction() * 0.01f);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera.move(m_camera.direction() * -0.01f);

    /*if (m_selectedModelInd < 0)
        return;

    Core3d::Model3d& selectedModel = m_models[m_selectedModelInd];

    if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) {
        const Core3d::Affine3d::Mat transform =
            Core3d::Affine3d::rotationAroundPoint(m_axesCenter, m_rotationParam);
        selectedModel.applyTransform(transform);

        if (m_selectedModelInd == m_axesModelInd)
            m_axesAngle += m_rotationParam;
    }
    if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) {
        const Core3d::Affine3d::Mat transform =
            Core3d::Affine3d::rotationAroundPoint(m_axesCenter, -m_rotationParam);
        selectedModel.applyTransform(transform);

        if (m_selectedModelInd == m_axesModelInd)
            m_axesAngle -= m_rotationParam;
    }

    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
        const Core3d::Affine3d::Mat transform =
            Core3d::Affine3d::scaleByAxes(m_axesCenter, m_axesAngle, Coord3d(1.0f + m_scaleParam, 1.0f));
        selectedModel.applyTransform(transform);
    }
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
        const Core3d::Affine3d::Mat transform =
            Core3d::Affine3d::scaleByAxes(m_axesCenter, m_axesAngle, Coord3d(1.0f, 1.0f + m_scaleParam));
        selectedModel.applyTransform(transform);
    }
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
        const Core3d::Affine3d::Mat transform =
            Core3d::Affine3d::scaleByAxes(m_axesCenter, m_axesAngle, Coord3d(1.0f - m_scaleParam, 1.0f));
        selectedModel.applyTransform(transform);
    }
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
        const Core3d::Affine3d::Mat transform =
            Core3d::Affine3d::scaleByAxes(m_axesCenter, m_axesAngle, Coord3d(1.0f, 1.0f - m_scaleParam));
        selectedModel.applyTransform(transform);
    }

    if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
        const Core3d::Affine3d::Mat transform =
            Core3d::Affine3d::scaleByAxes(m_axesCenter, m_axesAngle, Coord3d(1.0f + m_scaleParam, 1.0f + m_scaleParam));
        selectedModel.applyTransform(transform);
    }
    if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_PRESS) {
        const Core3d::Affine3d::Mat transform =
            Core3d::Affine3d::scaleByAxes(m_axesCenter, m_axesAngle, Coord3d(1.0f - m_scaleParam, 1.0f - m_scaleParam));
        selectedModel.applyTransform(transform);
    }*/
}

void Scene3d::resizeCallback(
    GLFWwindow* window,
    const int newWidth, const int newHeight)
{
    auto* scenePtr = (Scene3d*)glfwGetWindowUserPointer(window);
    const int pixelSize = scenePtr->m_pixelSize;
    scenePtr->m_width = newWidth / pixelSize;
    scenePtr->m_height = newHeight / pixelSize;
    scenePtr->m_camera.resizeKeepTopBottom(scenePtr->m_width, scenePtr->m_height);
    glViewport(
        0, 0,
        scenePtr->m_width * pixelSize,
        scenePtr->m_height * pixelSize);
}

void Scene3d::cursorPosCallback(
    GLFWwindow* window,
    const double newX, const double newY)
{
    auto* scenePtr = (Scene3d*)glfwGetWindowUserPointer(window);
    const Coord2d newCursorPos = Coord2d(newX, newY) / (float)scenePtr->m_pixelSize;

    if (scenePtr->m_camRotationOn) {
        const Coord2d screenDxy = newCursorPos - scenePtr->m_cursorPos;
        const float dYaw = screenDxy.x * 180.0f / scenePtr->m_camera.screenWidth();
        const float dPitch = screenDxy.y * 180.0f / scenePtr->m_camera.screenHeight();
        scenePtr->m_camera.rotate(dYaw, dPitch);
    }

    /*if (scenePtr->m_isPanning) {
        const Coord3d screenDxy = newCursorPos - scenePtr->m_cursorPos;
        scenePtr->m_camera.move(screenDxy);
    }

    const int selectedModelInd = scenePtr->m_selectedModelInd;
    if (scenePtr->m_isTranslating && selectedModelInd >= 0) {
        const Coord3d worldCursorPos =
            scenePtr->m_camera.screenToWorld(scenePtr->m_cursorPos);
        const Coord3d worldNewCursorPos =
            scenePtr->m_camera.screenToWorld(newCursorPos);
        const Coord3d worldDxy = worldNewCursorPos - worldCursorPos;
        const Core3d::Affine3d::Mat transform =
            Core3d::Affine3d::translation(worldDxy);
        scenePtr->m_models[selectedModelInd].applyTransform(transform);

        if (selectedModelInd == scenePtr->m_axesModelInd)
            scenePtr->m_axesCenter += worldDxy;
    }*/

    scenePtr->m_cursorPos = newCursorPos;
}

void Scene3d::scrollCallback(
    GLFWwindow* window,
    const double dx, const double dy)
{
    auto* scenePtr = (Scene3d*)glfwGetWindowUserPointer(window);
}

void Scene3d::mouseButtonCallback(
    GLFWwindow* window,
    const int button, const int action, const int mods)
{
    auto* scenePtr = (Scene3d*)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        scenePtr->m_camRotationOn = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        scenePtr->m_camRotationOn = false;
    /*if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
        scenePtr->m_isPanning = true;
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
        scenePtr->m_isPanning = false;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        const HomogCoord3d newVertex = scenePtr->m_camera.screenToWorld(
            scenePtr->m_cursorPos);
        scenePtr->m_verticesInProgress.push_back(newVertex);
        const int nVertices = scenePtr->m_verticesInProgress.size();
        scenePtr->m_adjacencyInProgress = Core3d::makePolylineAdjacency(nVertices);
    }*/
}

void Scene3d::keyCallback(
    GLFWwindow* window,
    const int key, const int scancode, const int action, const int mods)
{
    auto* scenePtr = (Scene3d*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(scenePtr->m_window, true);
    /*if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        scenePtr->m_camera.reset();
    if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE
        && scenePtr->m_verticesInProgress.size() >= 2) {
        scenePtr->m_models.push_back(
            Core3d::Model3d(
                scenePtr->m_verticesInProgress,
                scenePtr->m_adjacencyInProgress));
        scenePtr->m_modelColors.push_back(Rasterization::colorYellow);
        scenePtr->m_verticesInProgress.clear();
    }

    const bool hasSelection = scenePtr->m_selectedModelInd >= 0;
    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
        scenePtr->m_selectedModelInd = hasSelection ? -1 : 0;
        return;
    }

    if (!hasSelection)
        return;
    const int nModels = scenePtr->m_models.size();
    const int selectedModelInd = scenePtr->m_selectedModelInd;
    Core3d::Model3d& selectedModel = scenePtr->m_models[selectedModelInd];
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
        scenePtr->m_selectedModelInd = (selectedModelInd + 1) % nModels;
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
        scenePtr->m_selectedModelInd = (nModels + selectedModelInd - 1) % nModels;
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
        selectedModel.resetTransform();*/
}
