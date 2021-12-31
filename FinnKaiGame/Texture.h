#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <glm/glm.hpp>

class Texture
{
public:
	Texture(const std::string name);

	~Texture() { SDL_DestroyTexture(m_texture); }
	void createTexture(const std::string& path);
	SDL_Texture* getSDLTexture() { return m_texture; }
	glm::vec2 getSize() const { return m_size; }

private:
	SDL_Texture* m_texture;
	std::string m_name;
	glm::vec2 m_size;
};

