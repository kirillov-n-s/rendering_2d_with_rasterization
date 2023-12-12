cmake_minimum_required (VERSION 3.8)
project(laba1)

set(CMAKE_CXX_STANDARD 20)

add_executable(laba1 main.cpp
	"scene2d.h" "scene2d.cpp"

	"core2d/camera2d.h" "core2d/camera2d.cpp"
	"core2d/model2d.h" "core2d/model2d.cpp"
	"core2d/affine2d.h" "core2d/affine2d.cpp"

	"rasterization/bitmap.h" "rasterization/bitmap.cpp"
	"rasterization/color.h"
	"rasterization/bresenham.h" "rasterization/bresenham.cpp"
	"rasterization/wireframe.h" "rasterization/wireframe.cpp"
	"rasterization/scanline.h" "rasterization/scanline.cpp"
	"rasterization/polygonal.cpp" "rasterization/polygonal.h"
	"rasterization/shading.h" "rasterization/shading.cpp"

	"rendering/rendering.h" "rendering/rendering.cpp"
	
	"common_types.h" "common_types.cpp"

	"scene3d.h" "scene3d.cpp"
	
	"core3d/affine3d.cpp" "core3d/affine3d.h"
	"core3d/camera3d.cpp" "core3d/camera3d.h"
	"core3d/model3dbase.cpp" "core3d/model3dbase.h"
	"core3d/model3dwireframe.cpp" "core3d/model3dwireframe.h"
	"core3d/model3dpoly.h" "core3d/model3dpoly.cpp"
	 
	
	"io/obj.h" "io/obj.cpp"
)

target_link_libraries(laba1 opengl32.lib)
target_link_libraries(laba1 glew32.lib)
target_link_libraries(laba1 glfw3.lib)
