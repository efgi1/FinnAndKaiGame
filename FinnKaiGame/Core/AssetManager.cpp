#include "AssetManager.h"
#include <fstream>
#include <iostream>
#include "GameEngine.h"
#include "Texture.h"
#include <pugixml.hpp>

AssetManager::~AssetManager()
{
}

void AssetManager::loadFromFile(const std::string& path)
{

    std::cout << "\nParsing config data (sample.xml).....\n\n";

    pugi::xml_document doc;
    // load the XML file
    if (!doc.load_file(".\\config.xml")) return;

    pugi::xml_node textures = doc.child("GameConfig").child("Textures");
    pugi::xml_node animations = doc.child("GameConfig").child("Animations");


    for (pugi::xml_node_iterator it = textures.begin(); it != textures.end(); ++it)
    {
        std::cout << "Textures:";

        std::string name, path;
        for (pugi::xml_attribute_iterator ait = it->attributes_begin();
            ait != it->attributes_end(); ++ait)
        {
            std::string nameTag = ait->name();
            std::string value = ait->value();
            std::cout << " " << nameTag <<
                "=" << value;
            if (nameTag == "Name")
                name = value;
            if (nameTag == "FilePath")
                path = value;
        }
        addTexture(name, path);
        std::cout << std::endl;
    }

    for (pugi::xml_node_iterator it = animations.begin(); it != animations.end(); ++it)
    {
        std::cout << "Animations:";

        std::string name, texture;
        int frames = 0;
        float speed = 0;
        for (pugi::xml_attribute_iterator ait = it->attributes_begin();
            ait != it->attributes_end(); ++ait)
        {
            std::string nameTag = ait->name();
            std::string value = ait->value();
            std::cout << " " << nameTag <<
                "=" << value;
            if (nameTag == "Name")
                name = value;
            if (nameTag == "Texture")
                texture = value;
            if (nameTag == "Frames")
                frames = stoi(value);
            if (nameTag == "Speed")
                speed = stof(value);
        }
        addAnimation(name, texture, frames, speed);
        std::cout << std::endl;
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

void AssetManager::shutdown()
{
    for (auto texture : m_textureMap)
    {
        SDL_DestroyTexture(texture.second->getSDLTexture());
    }
}

void AssetManager::addTexture(const std::string& textureName, const std::string& path, bool smooth)
{
  std::shared_ptr<Texture> texture = std::make_shared<Texture>("kai");
  texture->createTexture(path);
  glm::vec2 getSize = texture->getSize();
  m_textureMap[textureName] = texture;
}

void AssetManager::addAnimation(const std::string& animationName, const std::string& textureName, size_t frameCount, float speed)
{
    auto getSize = m_textureMap[textureName]->getSize();
    std::shared_ptr<Animation> animation = std::make_unique<Animation>(animationName, getSize, frameCount, speed);
    m_animationMap[animationName] = animation;
}
