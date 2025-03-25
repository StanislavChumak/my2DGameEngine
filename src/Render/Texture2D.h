#ifndef H_TEXTURE2D
#define H_TEXTURE2D

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <string>
#include <map>

namespace Render
{
    class Texture2D
    {
    public:
        struct SubTexture2D
        {
            glm::vec2 leftBottomVertex = glm::vec2(0.0f);
            glm::vec2 rightTopVertex = glm::vec2(1.0f);

            SubTexture2D(glm::vec2 aLeftBottomVertex, glm::vec2 aRigchTopVertex)
            : leftBottomVertex(aLeftBottomVertex), rightTopVertex(aRigchTopVertex)
            {}

            SubTexture2D(){}
        };

        Texture2D(const GLuint width, const GLuint height,
                  const unsigned char *data, const int channels = 4,
                  const unsigned char nuber = 0,
                  const GLenum filter = GL_LINEAR, const GLenum wrapMode = GL_CLAMP_TO_EDGE);
        Texture2D() = delete;
        Texture2D(Texture2D &) = delete;
        Texture2D &operator=(const Texture2D &) = delete;
        Texture2D &operator=(Texture2D &&Texture2D);
        Texture2D(Texture2D &&Texture2D);
        ~Texture2D();

        void addSubTexture(const std::string subTextureName, const glm::vec2 leftBottomVertex, const glm::vec2 rightTopVertex);
        const SubTexture2D& getSubTexture(const std::string& subTextureName) const;
        unsigned int getWidth() const { return m_width;}
        unsigned int getHeight() const { return m_height;}
        void bind() const;
        unsigned char getNumber() const;

    private:
        GLuint m_ID = 0;
        unsigned char m_number = 0;
        GLenum m_mode = GL_RGBA;
        GLuint m_width = 0, m_height = 0;

        std::map<const std::string, SubTexture2D> m_subTextures;
    };
}

#endif