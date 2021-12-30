#pragma once
#include <map>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "Animation.h"

class AssetManager
{
public:

  AssetManager() {}
  ~AssetManager();

  void loadFromFile(const std::string& path);

  SDL_Texture* getTexture(const std::string& textureName) const;
  const Animation& getAnimation(const std::string& animationName) const;
  //const sf::Font& getFont(const std::string& fontName) const;

private:
  std::map<std::string, SDL_Texture*>      m_textureMap;
  std::map<std::string, Animation>        m_animationMap;
  //std::map<std::string, SDL_Font>         m_fontMap;

  void addTexture(const std::string& textureName, const std::string& path, bool smooth = true);
  void addAnimation(const std::string& animationName, const std::string& textureName, size_t frameCount, size_t speed);
  //void addFont(const std::string& fontName, const std::string& path);
};