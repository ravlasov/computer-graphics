cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(include)

add_executable(lab6 main.cpp 
	       Shader.cpp 
	       Camera.cpp 
	       toaster.cpp
	       axes.cpp
	       include/imgui/imgui_impl_glfw.cpp
	       include/imgui/imgui_impl_opengl3.cpp
	       include/imgui/imgui_draw.cpp
	       include/imgui/imgui_widgets.cpp
	       include/imgui/imgui.cpp)

       target_link_libraries(lab6 glfw GLEW OpenGL::GL OpenGL::GLU /usr/local/lib/libSOIL.a)
