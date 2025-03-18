#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "render/ShaderProgram.h"
#include "resources/ResourceManager.h"

void windowSizeCallback(GLFWwindow *, int, int);
void processInput(GLFWwindow *);

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;


GLfloat position[] = {
    0.5f, 0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, 0.5f, 0.0f, // top left
    -0.5f, -0.5f, 0.0f // bottom left
};
GLfloat color[] = {
    1.0f, 0.0f, 0.0f, //
    0.0f, 1.0f, 0.0f, //
    0.0f, 0.0f, 1.0f, //
    1.0f, 0.0f, 0.0f  //
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

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "my Window", nullptr, nullptr);
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

    auto defaultShaderProgram = resourceManager->loadShaders("DefaultShader","res/shaders/shader.vshader","res/shaders/shader.fshader");
    if(!defaultShaderProgram->isCompiled())
    {
        std::cerr << "Can't Create Sheder Program" << std::endl;
        return -1;
    }

    GLuint VBOposition, VBOcolor, VAO, EBO;
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBOposition);
    glGenBuffers(1, &VBOcolor);
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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        glClear(GL_COLOR_BUFFER_BIT);

        defaultShaderProgram->use();
        glBindVertexArray(VAO);
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
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
