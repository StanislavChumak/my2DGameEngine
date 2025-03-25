#include "Texture2D.h"

namespace Render
{
    Texture2D::Texture2D(const GLuint width, const GLuint height,
                         const unsigned char *data, const int channels,
                         const unsigned char number,
                         const GLenum filter, const GLenum wrapMode)
                         : m_width(width), m_height(height), m_number(number)
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
        glActiveTexture(GL_TEXTURE0 + m_number);
        glBindTexture(GL_TEXTURE_2D,m_ID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data);

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
        m_height = texture2D.m_height;
        m_number = texture2D.m_number;
        return *this;
    }

    Texture2D::Texture2D(Texture2D &&texture2D)
    {
        m_ID = texture2D.m_ID;
        texture2D.m_ID = 0;
        m_mode = texture2D.m_mode;
        m_width = texture2D.m_width;
        m_height = texture2D.m_height;
        m_number = texture2D.m_number;
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &m_ID);
    }

    void Texture2D::addSubTexture(const std::string subTextureName, const glm::vec2 leftBottomVertex, const glm::vec2 rightTopVertex)
    {
        m_subTextures.emplace(subTextureName, SubTexture2D(leftBottomVertex, rightTopVertex));
    }
    
    const Texture2D::SubTexture2D& Texture2D::getSubTexture(const std::string& subTextureName) const
    {
        auto iterator = m_subTextures.find(subTextureName);
        if(iterator != m_subTextures.end())
        {
            return iterator->second;
        }
        const static SubTexture2D defaultSubTexture;
        return defaultSubTexture;
    }

    void Texture2D::bind() const
    {
        glBindTexture(GL_TEXTURE_2D,m_ID);
    }

    unsigned char Texture2D::getNumber() const
    {
        return m_number;
    }
}