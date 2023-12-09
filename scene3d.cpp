#include <sstream>
#include <string>
#include <glm/trigonometric.hpp>
#include "rasterization/wireframe.h"
#include "rasterization/polygonal.h"
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

void Scene3d::setAxisModels(
    const std::array<Core3d::Model3dWireframe, 3>& models,
    const std::array<Rasterization::Color, 3>& colors)
{
    m_axisModels = std::vector(models.begin(), models.end());
    m_axisColors = std::vector(colors.begin(), colors.end());
}

void Scene3d::addModel(
    const Core3d::Model3dWireAndPoly& model,
    const Rasterization::Color& color)
{
    m_models.push_back(model);
    std::vector<Rasterization::Color> colorPerTriangle(model.triangleVertexIndices().size() / 3);
    std::generate(colorPerTriangle.begin(), colorPerTriangle.end(), Rasterization::colorRandom);
    m_triangleColorsPerModel.push_back(colorPerTriangle);
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
    const Coord3d cameraDir = m_camera.front();
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
    Rasterization::Bitmap zBuffer(m_width, m_height, Rasterization::depthMinValue, m_pixelSize);
    Rasterization::Bitmap optionalDepthMap(m_width, m_height, Rasterization::colorBlack, m_pixelSize);

    const int nModels = m_models.size();
    for (int modelInd = 0; modelInd < nModels; ++modelInd) {
        const Core3d::Model3dWireAndPoly& model = m_models[modelInd];
        const HomogCoords3d &vertices = model.vertices();
        const IndexVec& triangleVertexIndices = model.triangleVertexIndices();
        const Coords3d &screenVerticesWithDepth = m_camera.worldToScreenWithDepth(vertices);
        const std::vector<Rasterization::Color> &colorPerTriangle = m_triangleColorsPerModel[modelInd];
        if (m_showColors && m_showDepth) {
            Rasterization::rasterizeTrianglesWithZBuffer(
                screenVerticesWithDepth,
                triangleVertexIndices,
                colorPerTriangle,
                bitmap,
                zBuffer);
            Rasterization::zBufferToDepthMap(zBuffer, optionalDepthMap);
            bitmap.floatMultiplySelf(optionalDepthMap);
        }
        else if (m_showColors) {
            Rasterization::rasterizeTrianglesWithZBuffer(
                screenVerticesWithDepth,
                triangleVertexIndices,
                colorPerTriangle,
                bitmap,
                zBuffer);
        }
        else if (m_showDepth) {
            Rasterization::rasterizeDepthMap(
                screenVerticesWithDepth,
                triangleVertexIndices,
                bitmap,
                zBuffer);
        }
    }

    if (m_showAxes) {
        const int nAxes = m_axisModels.size();
        for (int axisInd = 0; axisInd < nAxes; ++axisInd) {
            const Core3d::Model3dWireframe& axisModel = m_axisModels[axisInd];
            const HomogCoords3d& vertices = axisModel.vertices();
            const AdjacencyMat& adjacency = axisModel.adjacency();
            std::vector<bool> isVertexVisible;
            const Coords2d& screenVertices = m_camera.worldToScreen(vertices, isVertexVisible);
            const AdjacencyMat& visibleVertexAdjacency = modifyAdjacency(
                adjacency, isVertexVisible,
                std::logical_and<>());
            const Rasterization::Color color = m_axisColors[axisInd];
            Rasterization::rasterizeWireframe(
                screenVertices,
                visibleVertexAdjacency,
                color,
                bitmap);
        }
    }

    Rendering::sendImageToFramebuffer(
        bitmap.width(), bitmap.height(),
        m_bufData, bitmap.data());
}

void Scene3d::handleTransform(const float frameTime)
{
    if (m_camTranslationOn) {

        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
            m_camera.move(m_camera.front() * m_movementSpeed);
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
            m_camera.move(m_camera.front() * -m_movementSpeed);
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
            m_camera.move(m_camera.right() * -m_movementSpeed);
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
            m_camera.move(m_camera.right() * m_movementSpeed);
        if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
            m_camera.move(m_camera.up() * m_movementSpeed);
        if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
            m_camera.move(m_camera.up() * -m_movementSpeed);
        return;
    }

    Core3d::Model3dWireframe& xAxisModel = m_models[0];
    Core3d::Model3dWireframe& yAxisModel = m_models[1];
    Core3d::Model3dWireframe& zAxisModel = m_models[2];
    Core3d::Model3dWireframe& selectedModel = m_models.back();

    if (m_modelOrPivotRotationOn) {
        const float radRotationSpeed = glm::radians(m_rotationSpeed) * 10.0f;
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
            const Coord3d vec = Directions3d::right * radRotationSpeed;
            const Core3d::Affine3d::Mat transform =
                Core3d::Affine3d::rotationWithPivot(m_pivotTranslation, m_pivotYawPitchRoll, vec);
            if (m_pivotTranslationOn) {
                m_pivotYawPitchRoll += vec;
                xAxisModel.applyTransform(transform);
                yAxisModel.applyTransform(transform);
                zAxisModel.applyTransform(transform);
            }
            else {
                selectedModel.applyTransform(transform);
            }
        }

        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
            const Coord3d vec = Directions3d::left * radRotationSpeed;
            const Core3d::Affine3d::Mat transform =
                Core3d::Affine3d::rotationWithPivot(m_pivotTranslation, m_pivotYawPitchRoll, vec);
            if (m_pivotTranslationOn) {
                m_pivotYawPitchRoll += vec;
                xAxisModel.applyTransform(transform);
                yAxisModel.applyTransform(transform);
                zAxisModel.applyTransform(transform);
            }
            else {
                selectedModel.applyTransform(transform);
            }
        }

        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
            const Coord3d vec = Directions3d::forward * radRotationSpeed;
            const Core3d::Affine3d::Mat transform =
                Core3d::Affine3d::rotationWithPivot(m_pivotTranslation, m_pivotYawPitchRoll, vec);
            if (m_pivotTranslationOn) {
                m_pivotYawPitchRoll += vec;
                xAxisModel.applyTransform(transform);
                yAxisModel.applyTransform(transform);
                zAxisModel.applyTransform(transform);
            }
            else {
                selectedModel.applyTransform(transform);
            }
        }

        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
            const Coord3d vec = Directions3d::backward * radRotationSpeed;
            const Core3d::Affine3d::Mat transform =
                Core3d::Affine3d::rotationWithPivot(m_pivotTranslation, m_pivotYawPitchRoll, vec);
            if (m_pivotTranslationOn) {
                m_pivotYawPitchRoll += vec;
                xAxisModel.applyTransform(transform);
                yAxisModel.applyTransform(transform);
                zAxisModel.applyTransform(transform);
            }
            else {
                selectedModel.applyTransform(transform);
            }
        }

        if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) {
            const Coord3d vec = Directions3d::up * radRotationSpeed;
            const Core3d::Affine3d::Mat transform =
                Core3d::Affine3d::rotationWithPivot(m_pivotTranslation, m_pivotYawPitchRoll, vec);
            if (m_pivotTranslationOn) {
                m_pivotYawPitchRoll += vec;
                xAxisModel.applyTransform(transform);
                yAxisModel.applyTransform(transform);
                zAxisModel.applyTransform(transform);
            }
            else {
                selectedModel.applyTransform(transform);
            }
        }

        if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) {
            const Coord3d vec = Directions3d::down * radRotationSpeed;
            const Core3d::Affine3d::Mat transform =
                Core3d::Affine3d::rotationWithPivot(m_pivotTranslation, m_pivotYawPitchRoll, vec);
            if (m_pivotTranslationOn) {
                m_pivotYawPitchRoll += vec;
                xAxisModel.applyTransform(transform);
                yAxisModel.applyTransform(transform);
                zAxisModel.applyTransform(transform);
            }
            else {
                selectedModel.applyTransform(transform);
            }
        }
    }
    else {
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
            const Coord3d vec = Directions3d::backward * m_movementSpeed;
            const Core3d::Affine3d::Mat transform = Core3d::Affine3d::translation(vec);
            if (m_pivotTranslationOn) {
                m_pivotTranslation += vec;
                xAxisModel.applyTransform(transform);
                yAxisModel.applyTransform(transform);
                zAxisModel.applyTransform(transform);
            }
            else {
                selectedModel.applyTransform(transform);
            }
        }

        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
            const Coord3d vec = Directions3d::forward * m_movementSpeed;
            const Core3d::Affine3d::Mat transform = Core3d::Affine3d::translation(vec);
            if (m_pivotTranslationOn) {
                m_pivotTranslation += vec;
                xAxisModel.applyTransform(transform);
                yAxisModel.applyTransform(transform);
                zAxisModel.applyTransform(transform);
            }
            else {
                selectedModel.applyTransform(transform);
            }
        }

        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
            const Coord3d vec = Directions3d::left * m_movementSpeed;
            const Core3d::Affine3d::Mat transform = Core3d::Affine3d::translation(vec);
            if (m_pivotTranslationOn) {
                m_pivotTranslation += vec;
                xAxisModel.applyTransform(transform);
                yAxisModel.applyTransform(transform);
                zAxisModel.applyTransform(transform);
            }
            else {
                selectedModel.applyTransform(transform);
            }
        }

        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
            const Coord3d vec = Directions3d::right * m_movementSpeed;
            const Core3d::Affine3d::Mat transform = Core3d::Affine3d::translation(vec);
            if (m_pivotTranslationOn) {
                m_pivotTranslation += vec;
                xAxisModel.applyTransform(transform);
                yAxisModel.applyTransform(transform);
                zAxisModel.applyTransform(transform);
            }
            else {
                selectedModel.applyTransform(transform);
            }
        }

        if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) {
            const Coord3d vec = Directions3d::up * m_movementSpeed;
            const Core3d::Affine3d::Mat transform = Core3d::Affine3d::translation(vec);
            if (m_pivotTranslationOn) {
                m_pivotTranslation += vec;
                xAxisModel.applyTransform(transform);
                yAxisModel.applyTransform(transform);
                zAxisModel.applyTransform(transform);
            }
            else {
                selectedModel.applyTransform(transform);
            }
        }

        if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) {
            const Coord3d vec = Directions3d::down * m_movementSpeed;
            const Core3d::Affine3d::Mat transform = Core3d::Affine3d::translation(vec);
            if (m_pivotTranslationOn) {
                m_pivotTranslation += vec;
                xAxisModel.applyTransform(transform);
                yAxisModel.applyTransform(transform);
                zAxisModel.applyTransform(transform);
            }
            else {
                selectedModel.applyTransform(transform);
            }
        }
    }
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

    const Coord2d screenDxy = newCursorPos - scenePtr->m_cursorPos;
    const float dYaw = screenDxy.x * scenePtr->m_rotationSpeed;
    const float dPitch = -screenDxy.y * scenePtr->m_rotationSpeed;
    if (scenePtr->m_camRotationOn)
        scenePtr->m_camera.rotate(dYaw, dPitch);

    scenePtr->m_cursorPos = newCursorPos;
}

void Scene3d::scrollCallback(
    GLFWwindow* window,
    const double dx, const double dy)
{
    auto* scenePtr = (Scene3d*)glfwGetWindowUserPointer(window);
    const float scaleFactor = dy * 0.1f;
    Core3d::Model3dWireframe& selectedModel = scenePtr->m_models.back();
    const Coord3d& pivotTranslation = scenePtr->m_pivotTranslation;
    const Coord3d& pivotYawPitchRoll = scenePtr->m_pivotYawPitchRoll;
    if (scenePtr->m_modelXScalingOn) {
        const Coord3d vec = Directions3d::right * scaleFactor + Directions3d::one;
        const Core3d::Affine3d::Mat transform =
            Core3d::Affine3d::scaleWithPivot(pivotTranslation, pivotYawPitchRoll, vec);
        selectedModel.applyTransform(transform);
    }
    if (scenePtr->m_modelYScalingOn) {
        const Coord3d vec = Directions3d::up * scaleFactor + Directions3d::one;
        const Core3d::Affine3d::Mat transform =
            Core3d::Affine3d::scaleWithPivot(pivotTranslation, pivotYawPitchRoll, vec);
        selectedModel.applyTransform(transform);
    }
    if (scenePtr->m_modelZScalingOn) {
        const Coord3d vec = Directions3d::forward * scaleFactor + Directions3d::one;
        const Core3d::Affine3d::Mat transform =
            Core3d::Affine3d::scaleWithPivot(pivotTranslation, pivotYawPitchRoll, vec);
        selectedModel.applyTransform(transform);
    }
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
}

void Scene3d::keyCallback(
    GLFWwindow* window,
    const int key, const int scancode, const int action, const int mods)
{
    auto* scenePtr = (Scene3d*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(scenePtr->m_window, true);
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        scenePtr->m_camTranslationOn = false;
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
        scenePtr->m_camTranslationOn = true;
    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
        scenePtr->m_camTranslationOn = false;
        scenePtr->m_pivotTranslationOn = true;
    }
    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
        scenePtr->m_camTranslationOn = true;
        scenePtr->m_pivotTranslationOn = false;
    }
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
        scenePtr->m_modelOrPivotRotationOn = true;
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE)
        scenePtr->m_modelOrPivotRotationOn = false;

    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        scenePtr->m_modelXScalingOn = true;
    if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
        scenePtr->m_modelXScalingOn = false;
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        scenePtr->m_modelYScalingOn = true;
    if (key == GLFW_KEY_X && action == GLFW_RELEASE)
        scenePtr->m_modelYScalingOn = false;
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        scenePtr->m_modelZScalingOn = true;
    if (key == GLFW_KEY_C && action == GLFW_RELEASE)
        scenePtr->m_modelZScalingOn = false;

    if (key == GLFW_KEY_1 && action == GLFW_RELEASE) {
        scenePtr->m_showColors = true;
        scenePtr->m_showDepth = false;
    }
    if (key == GLFW_KEY_2 && action == GLFW_RELEASE) {
        scenePtr->m_showColors = false;
        scenePtr->m_showDepth = true;
    }
    if (key == GLFW_KEY_3 && action == GLFW_RELEASE) {
        scenePtr->m_showColors = true;
        scenePtr->m_showDepth = true;
    }
    if (key == GLFW_KEY_TAB && action == GLFW_RELEASE)
        scenePtr->m_showAxes ^= true;
}
