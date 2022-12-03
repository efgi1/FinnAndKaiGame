#include "GameEngine.h"
#include "TestScene.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "imgui_internal.h"
#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "InputComponent.h"



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
	networkManager()->init();
	
}

void GameEngine::update()
{
	m_sceneManager->update();
	networkManager()->update();
}

void GameEngine::updatePlayer(message& m)
{
	auto view = m_entityManager->view<std::string, CTransform>();
	bool updated = false;
	for (auto [entity, name, transform] : view.each())
	{
		if (name == std::string(m.name))
		{
			updated = true;
			transform.pos.x = m.x;
			transform.pos.y = m.y;

		}
	}

	if (!updated)
	{
		auto newPlayer = m_entityManager->create();
		m_entityManager->emplace<std::string>(newPlayer, m.name);
		m_entityManager->emplace<CTransform>(newPlayer, glm::vec2(m.x, m.y));
		m_entityManager->emplace<CAnimation>(newPlayer, "kai", *(GameEngine::instance()->assetManager()->getAnimation("kai_standing")), false);
	}
}


void GameEngine::run()
{
	
	m_running = true;
	m_assetManager->loadFromFile(".\\config.txt");

	m_sceneManager->setCurrentScene(std::make_unique<TestScene>());

	while (m_running)
	{
		update();
		while (!networkManager()->m_messageQ.empty())
		{
			auto test = networkManager()->m_messageQ.front();
			networkManager()->m_messageQ.pop();
			if (test.type == 1)
			{
				bool newPlayerEntity = m_entityManager->empty();
				if (newPlayerEntity)
				{
					auto newEntity = m_entityManager->create();
					playerName = test.name;
					m_entityManager->emplace<std::string>(newEntity, test.name);
					m_entityManager->emplace<CTransform>(newEntity, glm::vec2(test.x, test.y));
					m_entityManager->emplace<CAnimation>(newEntity, "kai", *(GameEngine::instance()->assetManager()->getAnimation("kai_standing")), false);
					m_entityManager->emplace<CInput>(newEntity);
				}
				else if (playerName != std::string(test.name))
				{
					updatePlayer(test);
				}
			}
			if (test.type == 2)
			{
				auto view = m_entityManager->view<std::string>();
				entt::entity toRemove;
				for (auto [entity, name] : view.each())
				{
					if (name == std::string(test.name))
					{
						toRemove = entity;
						break;

					}
				}
				m_entityManager->destroy(toRemove);
			}
		}
		auto view = m_entityManager->view<std::string, CTransform>();
		for (auto [entity, name, transform] : view.each())
		{
			if (name == playerName && (transform.pos.x != transform.prevPos.x || transform.pos.y != transform.prevPos.y))
			{
				message m;
				m.x = transform.pos.x;
				m.y = transform.pos.y;
				strcpy(m.name,playerName.c_str());
				networkManager()->SendPlayerUpdate(m);
			}
		}
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		bool yes = true;
		ImGui::ShowDemoWindow(&yes);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				//ImGui::MenuItem("(demo menu)", NULL, false, false);
				if (ImGui::MenuItem("New")) {}
				if (ImGui::MenuItem("Open", "Ctrl+O")) {}
				if (ImGui::MenuItem("Save", "Ctrl+S")) {}
				if (ImGui::MenuItem("Save As..")) {}
				if (ImGui::MenuItem("Exit"))
				{
					GameEngine::instance()->quit();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			auto buttonWidth = ImGui::CalcTextSize("X").x + ImGui::GetStyle().ItemInnerSpacing.x * 2;
			auto frameWidth = ImGui::GetStyle().FramePadding.x;
			if (ImGui::CloseButton(1, ImVec2(GameEngine::instance()->WINDOW_WIDTH - buttonWidth - frameWidth, 0)))
			{
				GameEngine::instance()->quit();
			};
			ImGui::EndMainMenuBar();
		}

		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

		SDL_RenderPresent(renderer());
	}

	shutdown();
}

void GameEngine::quit()
{
	m_running = false;
}

void GameEngine::shutdown()
{

	networkManager()->CloseConnection();

	assetManager()->shutdown();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	//Quit SDL subsystems
	SDL_Quit();
	IMG_Quit();
}

bool GameEngine::isRunning()
{
	return m_running;
}



