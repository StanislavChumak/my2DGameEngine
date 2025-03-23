cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

set(PROJECT_NAME projectOpenGL)
project(${PROJECT_NAME} VERSION 0.1.0)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/Render/ShaderProgram.h
    src/Render/ShaderProgram.cpp
    src/Render/Texture2D.h
    src/Render/Texture2D.cpp
    src/Render/Sprite2D.h
    src/Render/Sprite2D.cpp
    src/resources/ResourceManager.h
    src/resources/ResourceManager.cpp
    src/resources/stb_image.h
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory (external/glad)
target_link_libraries(${PROJECT_NAME} glad)

include_directories(external/glm)

set_target_properties(${PROJECT_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
