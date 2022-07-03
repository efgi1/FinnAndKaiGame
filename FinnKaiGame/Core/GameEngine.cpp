#include "..\Core\GameEngine.h"
#include "..\TestScene.h"
#include "..\imgui\imgui.h"
#include "..\imgui\imgui_impl_sdl.h"
#include "..\imgui\imgui_impl_sdlrenderer.h"



void GameEngine::init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		throw "SDL could not be initialized. Error: %s\n", SDL_GetError();
	}
	IMG_Init(IMG_INIT_PNG);
	m_window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_SetWindowBordered(m_window, SDL_FALSE);
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr)
	{
		SDL_Log("Error creating SDL_Renderer");
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
	ImGui_ImplSDLRenderer_Init(m_renderer);

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

	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();


	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);

	m_window = nullptr;
	m_renderer = nullptr;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool GameEngine::isRunning()
{
	return m_running;
}



