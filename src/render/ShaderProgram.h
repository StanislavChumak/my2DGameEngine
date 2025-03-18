#ifndef H_SHADERPROGRAM
#define H_SHADERPROGRAM

#include <glad/glad.h>

namespace Render
{
    class ShaderProgram
    {
    public:
        ShaderProgram(const char *vertexShaderCode, const char *fragmentShaderCode);
        ShaderProgram(const ShaderProgram &) = delete;
        ShaderProgram &operator=(const ShaderProgram &) = delete;
        ShaderProgram &operator=(ShaderProgram &&shaderProgram) noexcept;
        ShaderProgram(ShaderProgram &&shaderProgram) noexcept;
        ~ShaderProgram();

        bool isCompiled() const { return m_isCompiled; };
        void use() const;
        void setInt(const char* name, GLint value);

    private:
        bool createShader(const char *sourse, GLenum shaderType, GLuint &shederID);
        bool m_isCompiled = false;
        GLuint m_ID = 0;
        
    };
}

#endif