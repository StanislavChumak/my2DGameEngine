#include "shaderProgram.h"

#include <string>
#include <fstream>
#include <iostream>

namespace render
{
    ShaderProgram::ShaderProgram(const char *vertexPath, const char *fragmentPath)
    {

        std::string vertexCode;
        std::string fragmentCode;
        {
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;

            std::string line;
            vShaderFile.open(vertexPath, std::ios::in);
            fShaderFile.open(fragmentPath, std::ios::in);

            if (vShaderFile.is_open() && fShaderFile.is_open())
            {
                while (std::getline(vShaderFile, line))
                {
                    vertexCode += line + "\n";
                }
                while (std::getline(fShaderFile, line))
                {
                    fragmentCode += line + "\n";
                }
                
            }
            else
            {
                std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ:" << std::endl;
            }

            vShaderFile.close();
            fShaderFile.close();
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        GLuint vertexShaderID;
        if (!createShader(vShaderCode, GL_VERTEX_SHADER, vertexShaderID))
        {
            std::cerr << "ERROR::VERTEX::SHADER::COMPILATION_FAILED" << std::endl;
            return;
        }

        GLuint fragmentShaderID;
        if (!createShader(fShaderCode, GL_FRAGMENT_SHADER, fragmentShaderID))
        {
            std::cerr << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED" << std::endl;
            glDeleteShader(vertexShaderID);
            return;
        }

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShaderID);
        glAttachShader(m_ID, fragmentShaderID);
        glLinkProgram(m_ID);
        int success;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[1024];
            glGetProgramInfoLog(m_ID, 1024, nullptr, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }
        else
        {
            m_isCompiled = true;
        }
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }

    bool ShaderProgram::createShader(const char *sourse, GLenum shaderType, GLuint &shederID)
    {
        shederID = glCreateShader(shaderType);
        glShaderSource(shederID, 1, &sourse, nullptr);
        glCompileShader(shederID);

        int success;
        glGetShaderiv(shaderType, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[1024];
            glGetShaderInfoLog(shederID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
            return false;
        }
        return true;
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(m_ID);
    }

    void ShaderProgram::use() const
    {
        glUseProgram(m_ID);
    }

    ShaderProgram &ShaderProgram::operator=(ShaderProgram &&shaderProgram) noexcept
    {
        glDeleteProgram(m_ID);
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;
        return *this;
    }

    ShaderProgram::ShaderProgram(ShaderProgram &&shaderProgram) noexcept
    {
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;
    }
}