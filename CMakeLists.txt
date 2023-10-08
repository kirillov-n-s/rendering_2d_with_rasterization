cmake_minimum_required (VERSION 3.8)
project(laba1)

set(CMAKE_CXX_STANDARD 20)

add_executable(laba1 main.cpp
	"scene2d.h" "scene2d.cpp"

	"core2d/camera2d.h" "core2d/camera2d.cpp"
	"core2d/model2d.h" "core2d/model2d.cpp"
	"core2d/affine2d.h" "core2d/affine2d.cpp"

	"rasterization/bitmap.h" "rasterization/bitmap.cpp"
	"rasterization/color.h" "rasterization/segment.h"
	"rasterization/bresenham.h" "rasterization/bresenham.cpp"

	"rendering/rendering.h" "rendering/rendering.cpp")

target_link_libraries(laba1 opengl32.lib)
target_link_libraries(laba1 glew32.lib)
target_link_libraries(laba1 glfw3.lib)


