#include "..\Core\AssetManager.h"
#include <fstream>
#include <iostream>
#include "..\Core\GameEngine.h"
#include "..\Common\Texture.h"

AssetManager::~AssetManager()
{
}

void AssetManager::loadFromFile(const std::string& path)
{
  std::ifstream file(path);
  std::string str;
  while (file.good())
  {
    file >> str;

    if (str == "Texture")
    {
      std::string name, path;
      file >> name >> path;
      addTexture(name, path);
    }
    else if (str == "Animation")
    {
      std::string name, texture;
      size_t frames;
      float speed;
      file >> name >> texture >> frames >> speed;
      addAnimation(name, texture, frames, speed);
    }
    /*else if (str == "Font")
    {
      std::string name, path;
      file >> name >> path;
      addFont(name, path);
    }*/
    else
    {
      std::cerr << "Unknown Asset Type: " << str << std::endl;
    }
  }
}

Texture* AssetManager::getTexture(const std::string& textureName) const
{
  return m_textureMap.at(textureName).get();
}

Animation* AssetManager::getAnimation(const std::string& animationName) const
{
  return  m_animationMap.at(animationName).get();
}

void AssetManager::addTexture(const std::string& textureName, const std::string& path, bool smooth)
{
  std::shared_ptr<Texture> texture = std::make_shared<Texture>("kai");
  texture->createTexture(path);
  glm::vec2 size = texture->getSize();
  m_textureMap[textureName] = texture;
}

void AssetManager::addAnimation(const std::string& animationName, const std::string& textureName, size_t frameCount, float speed)
{
    auto size = m_textureMap[textureName]->getSize();
    std::shared_ptr<Animation> animation = std::make_unique<Animation>(animationName, size, frameCount, speed);
    m_animationMap[animationName] = animation;
}
