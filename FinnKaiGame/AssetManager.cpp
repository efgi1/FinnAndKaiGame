#include "AssetManager.h"
#include <fstream>
#include <iostream>
#include "GameEngine.h"

AssetManager::~AssetManager()
{
  for (auto texture : m_textureMap)
  {
    SDL_DestroyTexture(texture.second);
  }
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
      size_t frames, speed;
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

SDL_Texture* AssetManager::getTexture(const std::string& textureName) const
{
  return m_textureMap.at(textureName);
}

const Animation& AssetManager::getAnimation(const std::string& animationName) const
{
  return  m_animationMap.at(animationName);
}

void AssetManager::addTexture(const std::string& textureName, const std::string& path, bool smooth)
{
  std::ifstream test(path);
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == nullptr)
  {
    std::cout << "Reason: " << SDL_GetError() << std::endl;
  }
  SDL_Texture* texture = SDL_CreateTextureFromSurface(GameEngine::instance()->renderer(), loadedSurface);
  m_textureMap[textureName] = texture;
  //TODO smooth up
  glm::vec2 size(loadedSurface->w, loadedSurface->h);
  Animation animation(textureName, size, 4, 0.025);
  m_animationMap[textureName] = animation;
  SDL_FreeSurface(loadedSurface);
}

void AssetManager::addAnimation(const std::string& animationName, const std::string& textureName, size_t frameCount, size_t speed)
{
  
}
