cmake_minimum_required (VERSION 3.8)
project(laba1)

set(CMAKE_CXX_STANDARD 20)

add_executable(laba1 main.cpp
	#"scene2d.h" "scene2d.cpp"

	#"core2d/camera2d.h" "core2d/camera2d.cpp"
	#"core2d/model2d.h" "core2d/model2d.cpp"
	#"core2d/affine2d.h" "core3d/affine3d.cpp"

	"rasterization/bitmap.h" "rasterization/bitmap.cpp"
	"rasterization/color.h"
	"rasterization/bresenham.h" "rasterization/bresenham.cpp"
	"rasterization/rasterization.h" "rasterization/rasterization.cpp"

	"rendering/rendering.h" "rendering/rendering.cpp"
	
	"common_types.h" "common_types.cpp"

	"scene3d.h" "scene3d.cpp"
	
	"core3d/affine3d.cpp" "core3d/affine3d.h"
	"core3d/camera3d.cpp" "core3d/camera3d.h"
	"core3d/model3d.cpp" "core3d/model3d.h"
	
	"io/obj.h" "io/obj.cpp"
 )

target_link_libraries(laba1 opengl32.lib)
target_link_libraries(laba1 glew32.lib)
target_link_libraries(laba1 glfw3.lib)
