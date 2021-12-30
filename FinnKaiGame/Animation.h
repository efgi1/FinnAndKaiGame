#pragma once
#include <glm/glm.hpp>
#include <SDL.h>
#include <string>
#include <vector>

class Animation
{
public:
  Animation() : m_name("none"), m_sprite(), m_frameCount(1), m_currentFrame(0), m_speed(1) {}
  Animation(const std::string& name, glm::vec2& size);
  Animation(const std::string& name, glm::vec2& size,size_t frameCount, size_t speed);
  ~Animation();

  void update();
  bool hasEnded() const;
  const std::string& getName() const;
  const SDL_Rect& getSprite() const;

private:
  std::string m_name;
  SDL_Rect m_sprite;
  size_t m_frameCount;
  size_t m_currentFrame;
  size_t m_speed;
};

