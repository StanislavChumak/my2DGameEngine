#include "ResourceManager.h"
#include "../render/ShaderProgram.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

ResourceManager *ResourceManager::m_resourceManager = nullptr;

ResourceManager *ResourceManager::getResourceManager(const std::string &executablePath)
{
    if (m_resourceManager == nullptr)
    {
        m_resourceManager = new ResourceManager(executablePath);
    }
    return m_resourceManager;
}

ResourceManager::ResourceManager(const std::string &executablePath)
{
    size_t found = executablePath.find_last_of("/\\");
    m_path = executablePath.substr(0, found);
}

std::shared_ptr<Render::ShaderProgram> ResourceManager::loadShaders(const std::string &nameShader, const std::string &vertexPath, const std::string &fragmentPath)
{
    std::string vertexShaderCode = getFileString(vertexPath);
    if (vertexShaderCode.empty())
    {
        std::cerr << "No vertex shader !" << std::endl;
        return nullptr;
    }

    std::string fragmentShaderCode = getFileString(fragmentPath);
    if (fragmentShaderCode.empty())
    {
        std::cerr << "No fragment shader !" << std::endl;
        return nullptr;
    }

    std::shared_ptr<Render::ShaderProgram> &newShader = m_shaderPrograms.emplace(nameShader, std::make_shared<Render::ShaderProgram>(vertexShaderCode.c_str(), fragmentShaderCode.c_str())).first->second;
    if(newShader->isCompiled())
    {
        return newShader;
    }

    std::cerr << "Can't load shader program: \n"
    << "Vertex: " << vertexPath << "\n"
    << "Fragment: " << fragmentPath << std::endl;

    return nullptr;
}

std::shared_ptr<Render::ShaderProgram> ResourceManager::getShaderProgram(const std::string &nameShader)
{
    ShaderProgramsMap::const_iterator iter = m_shaderPrograms.find(nameShader);
    if (iter != m_shaderPrograms.end())
    {
        return iter->second;
    }
    std::cerr << "Can't find the shader program: " << nameShader << std::endl;
    return nullptr;
}

std::string ResourceManager::getFileString(const std::string relativeFilePath) const
{
    std::ifstream fileFlow;
    fileFlow.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
    if (!fileFlow.is_open())
    {
        std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
        return std::string{};
    }

    std::stringstream stringBuffer;
    stringBuffer << fileFlow.rdbuf();
    return stringBuffer.str();
}

ResourceManager::~ResourceManager()
{
    m_resourceManager = nullptr;
}