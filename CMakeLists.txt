cmake_minimum_required(VERSION 3.13.0)

set(PROJECT_NAME newProjectOpenGL)
project(${PROJECT_NAME} VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/render/shaderProgram.h
    src/render/shaderProgram.cpp
)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory (external/glad)
target_link_libraries(${PROJECT_NAME} glad)