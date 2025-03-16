#ifndef H_SHADERPROGRAM
#define H_SHADERPROGRAM

#include <glad/glad.h>

namespace render
{
    class ShaderProgram
    {
    public:
        ShaderProgram(const char *vertexPath, const char *fragmentPath);
        ~ShaderProgram();
        bool isCompiled() const { return m_isCompiled; };
        void use() const;

        ShaderProgram() = default;
        ShaderProgram(ShaderProgram&) = default;
        ShaderProgram& operator=(const ShaderProgram&) = default;
        ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
        ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

    private:
        bool createShader(const char *sourse,GLenum shaderType, GLuint& shederID);
        bool m_isCompiled = false;
        GLuint m_ID = 0;
    };
}

#endif