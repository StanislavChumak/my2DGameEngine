#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "render/ShaderProgram.h"
#include "render/Texture2D.h"
#include "resources/ResourceManager.h"


void windowSizeCallback(GLFWwindow *, int, int);
void processInput(GLFWwindow *);

glm::vec2 windowSize = glm::vec2(800.0f, 600.0f);


const GLfloat position[] = {
    50.0f, 50.0f, 0.0f,  // top right
    50.0f, -50.0f, 0.0f, // bottom right
    -50.0f, 50.0f, 0.0f, // top left
    -50.0f, -50.0f, 0.0f // bottom left
};
const GLfloat color[] = {
    1.0f, 1.0f, 1.0f, //
    1.0f, 1.0f, 1.0f, //
    1.0f, 1.0f, 1.0f, //
    1.0f, 1.0f, 1.0f  //
};
const GLfloat texCor[] = {
    1.0f, 1.0f, //
    1.0f, 0.0f, //
    0.0f, 1.0f, //
    0.0f, 0.0f  //
};
GLuint indices[] = {
    // note that we start from 0!
    0, 1, 2, // first Triangle
    1, 2, 3  // second Triangle
};

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

    auto defaultShaderProgram = resourceManager->loadShaders("DefaultShader","res/shaders/vertex.vshader","res/shaders/fragment.fshader");
    if(!defaultShaderProgram->isCompiled())
    {
        std::cerr << "Can't Create Sheder Program" << std::endl;
        return -1;
    }

    auto mattressTexture = resourceManager->loadTexture2D("Logo","res/textures/mattress_logo_v1.png");

    GLuint VBOposition, VBOcolor, VBOtexture, VAO, EBO;
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBOposition);
    glGenBuffers(1, &VBOcolor);
    glGenBuffers(1, &VBOtexture);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOposition);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOcolor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBOtexture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCor), texCor, GL_STATIC_DRAW);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    defaultShaderProgram->use();
    defaultShaderProgram->setInt("Tex", 0);

    glm::mat4 projectionMatrix = glm::ortho(0.0f,windowSize.x,0.0f,windowSize.y,-100.0f,100.0f);
    defaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(400.0f, 300.0f, 0.0f));
        trans = glm::scale(trans, glm::vec3(3.0f - sin(glfwGetTime()), 3.0f - sin(glfwGetTime()), 3.0f));
        trans = glm::rotate(trans, (float)glfwGetTime() * glm::radians(100.0f), glm::vec3(0.0, 0.0, 1.0));
        defaultShaderProgram->setMatrix4("transform", trans);

        defaultShaderProgram->use();
        glBindVertexArray(VAO);
        mattressTexture->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBOposition);
    glDeleteBuffers(1, &EBO);

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
