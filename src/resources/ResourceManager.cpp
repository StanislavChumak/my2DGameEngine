#include "ResourceManager.h"
#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"
#include "../Render/Sprite2D.h"

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

std::shared_ptr<Render::Texture2D> ResourceManager::loadTexture2D(const std::string &textureName, const std::string &texturePath, const unsigned char number)
{
    int channels = 0, width = 0, height = 0;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

    if (!pixels)
    {
        std::cerr << "Can't load image: " << texturePath << std::endl;
        return nullptr;
    }

    std::shared_ptr<Render::Texture2D> &newTexture2D = m_textures2D.emplace(textureName, std::make_shared<Render::Texture2D>(width, height, pixels, channels, number, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

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

std::shared_ptr<Render::Texture2D> ResourceManager::loadTextureAtlas(const std::string &textureName, const std::string &texturePath,
    const std::vector<std::string> subTextureNames, unsigned int subTextureWidth, unsigned int subTextureHeight)
{
    auto texture = loadTexture2D(std::move(textureName),std::move(texturePath));
    if(texture)
    {
        const unsigned int textureWidth = texture->getWidth();
        const unsigned int textureHeight = texture->getHeight();
        unsigned int currentTextureOffsetX = 0;
        unsigned int currentTextureOffsetY = textureHeight;
        for(const auto& subTextureName : subTextureNames)
        {
            glm::vec2 leftBottom(static_cast<float>(currentTextureOffsetX) / textureWidth, static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);
            glm::vec2 rigthTop(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth, static_cast<float>(currentTextureOffsetY) / textureHeight);

            texture->addSubTexture(std::move(subTextureName), leftBottom, rigthTop);

            currentTextureOffsetX += subTextureWidth;
            if(currentTextureOffsetX >= textureWidth)
            {
                currentTextureOffsetX = 0;
                currentTextureOffsetY -= subTextureHeight;
            }
        }
    }
    return texture;
}

std::shared_ptr<Render::Sprite2D> ResourceManager::loadSprite2D(const std::string &spriteName, const std::string &shaderName, const std::string &textureName, const std::string &subTextureName, glm::vec3 color)
{
    auto shader = getShaderProgram(shaderName);
    if(!shader)
    {
        std::cerr << "Can't find the shaderProgram: " << shaderName << " for the sprite: " << spriteName << std::endl;
        return nullptr;
    }
    auto texture = getTexture2D(textureName);
    if(!texture)
    {
        std::cerr << "Can't find the texture2D: " << textureName << " for the sprite: " << spriteName << std::endl;
        return nullptr;
    }

    std::shared_ptr<Render::Sprite2D> &newSprite2D = m_sprites2D.emplace(spriteName, std::make_shared<Render::Sprite2D>(shader, texture, subTextureName, glm::vec2(0.0f), glm::vec2(0.0f), 0.0f, color)).first->second;

    return newSprite2D;
}

std::shared_ptr<Render::Sprite2D> ResourceManager::getSprite2D(const std::string &spriteName)
{
    Sprites2DMap::const_iterator iter = m_sprites2D.find(spriteName);
    if (iter != m_sprites2D.end())
    {
        return iter->second;
    }
    std::cerr << "Can't find the sprites2D: " << spriteName << std::endl;
    return nullptr;
}

ResourceManager::~ResourceManager()
{
    m_resourceManager = nullptr;
}