#ifndef H_RESOURCEMANADER
#define H_RESOURCEMANADER

#include <string>
#include <memory>
#include <map>

namespace Render
{
    class ShaderProgram;
    class Texture2D;
}

class ResourceManager
{
public:
    static ResourceManager *getResourceManager(const std::string &executablePath);
    ~ResourceManager();

    std::shared_ptr<Render::ShaderProgram> loadShaders(const std::string &shaderName, const std::string &vertexPath, const std::string &fragmentPath);
    std::shared_ptr<Render::ShaderProgram> getShaderProgram(const std::string &shaderName);

    std::shared_ptr<Render::Texture2D> loadTexture2D(const std::string &textureName, const std::string &texturePath);
    std::shared_ptr<Render::Texture2D> getTexture2D(const std::string &textureName);

private:
    static ResourceManager *m_resourceManager;
    ResourceManager(const std::string &executablePath);

    ResourceManager(ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    ResourceManager &operator=(ResourceManager &&resourceManager) noexcept = delete;
    ResourceManager(ResourceManager &&resourceManager) noexcept = delete;

    typedef std::map<const std::string, std::shared_ptr<Render::ShaderProgram>> ShaderProgramsMap;
    ShaderProgramsMap m_shaderPrograms;

    typedef std::map<const std::string, std::shared_ptr<Render::Texture2D>> Textures2DMap;
    Textures2DMap m_textures2D;

    std::string getFileString(const std::string relativeFilePath) const;

    std::string m_path;
};

#endif