#ifndef H_SPRITE2D
#define H_SPRITE2D

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <memory>

namespace Render
{
    class ShaderProgram;
    class Texture2D;

    class Sprite2D
    {
    public:
        Sprite2D(const std::shared_ptr<ShaderProgram> shaderProgram, const std::shared_ptr<Texture2D> texture,
               const glm::vec2 &position = glm::vec2(0.0f), const glm::vec2 &size = glm::vec2(1.0f),
               const float rotation = 0.0f, const glm::vec3 color = glm::vec3(1.0f));
        
        ~Sprite2D();

        Sprite2D(const Sprite2D&) = delete;
        Sprite2D operator=(const Sprite2D&) = delete;
        
        void render() const;
        void setPosition(const glm::vec2 &newPosition);
        void setSize(const glm::vec2 &newSize);
        void setRotation(const float newRototion);

    private:
        std::shared_ptr<ShaderProgram> m_shaderProgram;
        std::shared_ptr<Texture2D> m_texture;
        glm::vec2 m_position;
        glm::vec2 m_size;
        glm::vec3 m_color;
        float m_rotation;
        GLuint m_VAO;
        GLuint m_VBO[3];
        GLuint m_EBO;
    };
}

#endif