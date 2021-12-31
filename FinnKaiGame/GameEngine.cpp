#include "GameEngine.h"
#include "TestScene.h"


void GameEngine::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw "SDL could not be initialized. Error: %s\n", SDL_GetError();
	}

	m_window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	m_entityManager = std::make_unique<EntityManager>();
	m_assetManager = std::make_unique<AssetManager>();
	m_sceneManager = std::make_unique<SceneManager>();
	
}

void GameEngine::update()
{
	m_sceneManager->update();
}


void GameEngine::run()
{
	
	m_running = true;
	//Get window surface
	auto screenSurface = SDL_GetWindowSurface(m_window);

	//Fill the surface white
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 100, 100, 255));

	//Update the surface
	SDL_UpdateWindowSurface(m_window);
	m_assetManager->loadFromFile(".\\kai.txt");

	m_sceneManager->setCurrentScene(std::make_unique<TestScene>());
	while (m_running)
	{
		SDL_RenderClear(m_renderer);
		update();
	}
}

void GameEngine::quit()
{
	m_running = false;
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);

	//Quit SDL subsystems
	SDL_Quit();
}

bool GameEngine::isRunning()
{
	return m_running;
}



