cmake_minimum_required(VERSION 3.13.0)

set(PROJECT_NAME newProjectOpenGL)
project(${PROJECT_NAME} VERSION 0.1.0)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(glad)
target_link_libraries(${PROJECT_NAME} glad)