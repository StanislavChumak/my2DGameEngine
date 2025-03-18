#include "ResourceManager.h"
#include "../render/ShaderProgram.h"
#include "../render/Texture2D.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

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


std::shared_ptr<Render::ShaderProgram> ResourceManager::loadShaders(const std::string &shaderName, const std::string &vertexPath, const std::string &fragmentPath)
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

    std::shared_ptr<Render::ShaderProgram> &newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<Render::ShaderProgram>(vertexShaderCode.c_str(), fragmentShaderCode.c_str())).first->second;
    if (newShader->isCompiled())
    {
        return newShader;
    }

    std::cerr << "Can't load shader program: \n"
              << "Vertex: " << vertexPath << "\n"
              << "Fragment: " << fragmentPath << std::endl;

    return nullptr;
}

std::shared_ptr<Render::ShaderProgram> ResourceManager::getShaderProgram(const std::string &shaderName)
{
    ShaderProgramsMap::const_iterator iter = m_shaderPrograms.find(shaderName);
    if (iter != m_shaderPrograms.end())
    {
        return iter->second;
    }
    std::cerr << "Can't find the shader program: " << shaderName << std::endl;
    return nullptr;
}

std::shared_ptr<Render::Texture2D> ResourceManager::loadTexture2D(const std::string &textureName, const std::string &texturePath)
{
    int channels = 0, width = 0, height = 0;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

    if (!pixels)
    {
        std::cerr << "Can't load image: " << texturePath << std::endl;
        return nullptr;
    }

    std::shared_ptr<Render::Texture2D> &newTexture2D = m_textures2D.emplace(textureName, std::make_shared<Render::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

    stbi_image_free(pixels);
    return newTexture2D;
}

std::shared_ptr<Render::Texture2D> ResourceManager::getTexture2D(const std::string &textureName)
{
    Textures2DMap::const_iterator iter = m_textures2D.find(textureName);
    if (iter != m_textures2D.end())
    {
        return iter->second;
    }
    std::cerr << "Can't find the texture2D: " << textureName << std::endl;
    return nullptr;
}

ResourceManager::~ResourceManager()
{
    m_resourceManager = nullptr;
}