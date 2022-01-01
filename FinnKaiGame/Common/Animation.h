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
  Animation(const std::string& name, glm::vec2& size, size_t frameCount, float speed);
  ~Animation();

  void update();
  bool hasEnded() const;
  const std::string& getName() const;
  const SDL_Rect& getSprite() const;
  void setFlip(SDL_RendererFlip flip) { m_flip = flip; }
  SDL_RendererFlip getFlip() { return m_flip; }

private:
  std::string m_name;
  SDL_Rect m_sprite;
  SDL_RendererFlip m_flip = SDL_FLIP_NONE;
  size_t m_frameCount;
  float m_currentFrame;
  float m_speed;
};

