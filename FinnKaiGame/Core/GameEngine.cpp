#include "..\Core\GameEngine.h"
#include "..\TestScene.h"


void GameEngine::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw "SDL could not be initialized. Error: %s\n", SDL_GetError();
	}
	IMG_Init(IMG_INIT_PNG);
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
	m_assetManager->loadFromFile(".\\config.txt");

	m_sceneManager->setCurrentScene(std::make_unique<TestScene>());
	while (m_running)
	{
		update();
	}
}

void GameEngine::quit()
{
	m_running = false;
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool GameEngine::isRunning()
{
	return m_running;
}



