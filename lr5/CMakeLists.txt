cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(include)

add_executable(lab5 main.cpp Shader.cpp Camera.cpp)

target_link_libraries(lab5 glfw GLEW OpenGL::GL /usr/local/lib/libSOIL.a)
