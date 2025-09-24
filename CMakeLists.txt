#project name
set(PROJ_NAME "hello_window")
set(SRC "src/config.h" 
"src/main.cpp" 
"src/glad.c")


cmake_minimum_required(VERSION 3.6.0)
project(${PROJ_NAME} VERSION 0.0.1)

add_executable(${PROJ_NAME} ${SRC})

target_include_directories(${PROJ_NAME} PRIVATE dependencies)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJ_NAME} glfw OpenGL::GL)