#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Render/ShaderProgram.h"
#include "Render/Texture2D.h"
#include "Render/Sprite2D.h"
#include "resources/ResourceManager.h"


void windowSizeCallback(GLFWwindow *, int, int);
void processInput(GLFWwindow *);

glm::vec2 windowSize = glm::vec2(800.0f, 600.0f);

int main(int argc, char ** argv)
{
    if (!glfwInit())
    {
        std::cerr << "Failed GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(windowSize.x, windowSize.y, "my Window", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, windowSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Couidn't load opengl" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    // std::cout << "Processor: " << glGetString(GL_VENDOR) << std::endl;

    glClearColor(0.3f, 0.1f, 0.5f, 1.0f);



    ResourceManager* resourceManager = ResourceManager::getResourceManager(argv[0]);

    auto defaultShader = resourceManager->loadShaders("DefaultShader","res/shaders/vertex2D.vshader","res/shaders/fragment2D.fshader");
    if(!defaultShader->isCompiled())
    {
        std::cerr << "Can't Create Sheder Program" << std::endl;
        return -1;
    }
    defaultShader->use();


    auto defaultTexture = resourceManager->loadTexture2D("myTexture","res/textures/spikes_test_colors.png", 0);
    defaultShader->setInt("textureData", defaultTexture->getNumber());

    std::vector<std::string> subTexturesNemes = {"name", "void1", "void2","4"};
    auto TextureAtlas = resourceManager->loadTextureAtlas("DefaultTextureAtlas","res/textures/mattress_logo_v1.png", std::move(subTexturesNemes), 16, 16);


    auto defaultSprite = resourceManager->loadSprite2D("MySprte","DefaultShader","DefaultTextureAtlas", "name",glm::vec3(0.3f, 1.0f, 0.5f));
    defaultSprite->setSize(glm::vec2(300.0f, 300.0f));
    defaultSprite->setPosition(glm::vec2(windowSize.x / 2 - 150.0f, windowSize.y / 2 - 150.0f));


    glm::mat4 projectionMatrix = glm::ortho(0.0f,windowSize.x,0.0f,windowSize.y,-300.0f,300.0f);
    defaultShader->setMatrix4("projectionMatrix", projectionMatrix);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        glClear(GL_COLOR_BUFFER_BIT);

        defaultShader->use();
        defaultTexture->bind();
        
        defaultSprite->setRotation(30*sin(glfwGetTime()));
        defaultSprite->render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete resourceManager;

    glfwTerminate();

    return 0;
}

void windowSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    windowSize.x = width;
    windowSize.y = height;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
