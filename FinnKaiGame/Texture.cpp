#include "Texture.h"
#include "GameEngine.h"

Texture::Texture(const std::string name) :
	m_name(name),
	m_size(glm::vec2(0,0))
{
	m_texture = nullptr;
}

void Texture::createTexture(const std::string& path)
{
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		std::cout << "Error Loading texture: " << SDL_GetError() << std::endl;
	}
	m_texture = SDL_CreateTextureFromSurface(GameEngine::instance()->renderer(), loadedSurface);
	m_size = glm::vec2(loadedSurface->w, loadedSurface->h);
	SDL_FreeSurface(loadedSurface);
}

