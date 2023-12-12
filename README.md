University computer graphics project.

Implements fully functional 2d/3d camera, affine and projection transforms.
<br>Implements wireframe and triangle rasterization and rendering (Bresenham for lines, scanline with z-buffer for triangles).
<br>Only flat shading, no attribute interpolation or texture support (at least for now, as of `12.12.2023`).
<br>Writes rasterized image directly to framebuffer, no `OpenGL` shaders.

Comes with a minimal model set in `assets` folder (so `main.cpp` is fully functional without any external data).
<br> `assets` contain: a simple 500-triangle model, a large 50k-triangle model and a teapot.

Requirements:
* OpenGL
* GLFW (window library)
* GLEW (function loading library)
* GLM (math library)
