#include "Sprite2D.h"

#include "ShaderProgram.h"
#include "Texture2D.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Render
{
    Sprite2D::Sprite2D(const std::shared_ptr<ShaderProgram> shaderProgram, const std::shared_ptr<Texture2D> texture,
                   const glm::vec2 &position, const glm::vec2 &size, const float rotation, const glm::vec3 color)
        : m_shaderProgram(shaderProgram), m_texture(texture),
        m_position(position), m_size(size), m_rotation(rotation)
    {
        const GLfloat spriteData[] = {
            //vertex    //texture   //color
            1.0f, 1.0f, 1.0f, 1.0f, color.x, color.y, color.z,
            1.0f, 0.0f, 1.0f, 0.0f, color.x, color.y, color.z,
            0.0f, 1.0f, 0.0f, 1.0f, color.x, color.y, color.z,
            0.0f, 0.0f, 0.0f, 0.0f, color.x, color.y, color.z
        };
        const GLuint indices[] = {
            0, 1, 2, // first Triangle
            1, 2, 3  // second Triangle
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(3, m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(spriteData), spriteData, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *)0);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(spriteData), spriteData, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *)(2 * sizeof(GLfloat)));

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(spriteData), spriteData, GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *)(4 * sizeof(GLfloat)));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Sprite2D::~Sprite2D()
    {
        glDeleteBuffers(3, m_VBO);
        glDeleteBuffers(1, &m_EBO);
        glDeleteVertexArrays(1, &m_VAO);
    }

    void Sprite2D::render() const
    {
        m_shaderProgram->use();
        
        glm::mat4 model(1.0f);

        model = glm::translate(model, glm::vec3(0.5f * m_size.x + m_position.x, 0.5f * m_size.y + m_position.y, 0.0f));
        model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.0f));
        model = glm::scale(model, glm::vec3(m_size, 1.0f));

        glBindVertexArray(m_VAO);
        m_shaderProgram->setMatrix4("modelMatrix", model);
        glActiveTexture(GL_TEXTURE0 + m_texture->getNumber());
        m_texture->bind();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Sprite2D::setPosition(const glm::vec2 &newPosition)
    {
        m_position = newPosition;
    }

    void Sprite2D::setSize(const glm::vec2 &newSize)
    {
        m_size = newSize;
    }

    void Sprite2D::setRotation(const float newRotation)
    {
        m_rotation = newRotation;
    }
}
