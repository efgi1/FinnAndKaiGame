#include "Animation.h"

Animation::Animation(const std::string& name, glm::vec2& textureSize) : 
	Animation(name, textureSize, 1, 0)
{
}

Animation::Animation(const std::string& name, glm::vec2& textureSize, size_t frameCount, float speed) :
	m_name(name),
	m_frameCount(frameCount),
	m_speed(speed),
	m_currentFrame(0)
{
	m_sprite.w = static_cast<int>(textureSize.x / frameCount);
	m_sprite.h = static_cast<int>(textureSize.y);
	m_sprite.x = m_sprite.w;
	m_sprite.y = 0;

}

Animation::~Animation()
{
}

void Animation::update()
{
	m_sprite.x = static_cast<int>(m_currentFrame) % m_frameCount * (m_sprite.w);
	m_currentFrame += m_speed;
}

bool Animation::hasEnded() const
{
	return (m_currentFrame / m_speed) == m_frameCount;
}

const std::string& Animation::getName() const
{
	return m_name;
}

const SDL_Rect& Animation::getSprite() const
{
	return m_sprite;
}

