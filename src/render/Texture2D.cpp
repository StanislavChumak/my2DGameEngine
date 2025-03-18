#include "Texture2D.h"

namespace Render
{
    Texture2D::Texture2D(const GLuint width, const GLuint heigth,
                         const unsigned char *data, const int channels,
                         const GLenum filter, const GLenum wrapMode)
                         : m_width{width}, m_heigth{heigth}
    {
        switch (channels)
        {
        case 4:
            m_mode = GL_RGBA;
            break;
        case 3:
            m_mode = GL_RGB;
        }
        glGenTextures(1, &m_ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_ID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_heigth, 0, m_mode, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D,0);
    }
    Texture2D &Texture2D::operator=(Texture2D &&texture2D)
    {
        glDeleteTextures(1, &m_ID);
        m_ID = texture2D.m_ID;
        texture2D.m_ID = 0;
        m_mode = texture2D.m_mode;
        m_width = texture2D.m_width;
        m_heigth = texture2D.m_heigth;
        return *this;
    }

    Texture2D::Texture2D(Texture2D &&texture2D)
    {
        m_ID = texture2D.m_ID;
        texture2D.m_ID = 0;
        m_mode = texture2D.m_mode;
        m_width = texture2D.m_width;
        m_heigth = texture2D.m_heigth;
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &m_ID);
    }

    void Texture2D::bind() const
    {
        glBindTexture(GL_TEXTURE_2D,m_ID);
    }
}