#pragma once

#include <memory>
#include <vector>
#include <SDL.h>


#include "NetworkManager.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "AssetManager.h"

class GameEngine
{
public:
    ~GameEngine() { }

  const int WINDOW_WIDTH = 1960;
  const int WINDOW_HEIGHT = 1080;
  static GameEngine* instance() { 
    if (_instance == nullptr)
      _instance = std::unique_ptr<GameEngine>(new GameEngine);
    return _instance.get();
  }
 
  void run();
  void quit();
  void shutdown();

  //TODO renderingManager?
  SDL_Window* window() { return m_window; };
  SDL_Renderer* renderer() { return m_renderer; }
  EntityManager* entityManager() { return m_entityManager.get(); };
  AssetManager* assetManager() { return m_assetManager.get(); }
  SceneManager* sceneManager() { return m_sceneManager.get(); }
  NetworkManager* networkManager() { return NetworkManager::instance(); }
  bool isRunning();

private:
  inline static std::unique_ptr<GameEngine> _instance = nullptr;

  GameEngine() : m_running(true) { init(); }
  

  SDL_Window* m_window;
  SDL_Renderer* m_renderer;
  std::unique_ptr<AssetManager> m_assetManager;
  std::unique_ptr<EntityManager> m_entityManager;
  std::unique_ptr<SceneManager> m_sceneManager;
  std::string playerName;

  bool m_running;
  void init();
  void update();
  void updatePlayer(message& m);

 
};