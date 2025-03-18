#ifndef H_TEXTURE2D
#define H_TEXTURE2D

#include <glad/glad.h>
#include <string>

namespace Render
{
    class Texture2D
    {
    public:
        Texture2D(const GLuint width, const GLuint heigth,
                  const unsigned char *data, const int channels = 4,
                  const GLenum filter = GL_LINEAR, const GLenum wrapMode = GL_CLAMP_TO_EDGE);
        Texture2D() = delete;
        Texture2D(Texture2D &) = delete;
        Texture2D &operator=(const Texture2D &) = delete;
        Texture2D &operator=(Texture2D &&Texture2D);
        Texture2D(Texture2D &&Texture2D);
        ~Texture2D();

        void bind() const;

    private:
        GLuint m_ID = 0;
        GLenum m_mode = GL_RGBA;
        GLuint m_width = 0, m_heigth = 0;
    };
}

#endif