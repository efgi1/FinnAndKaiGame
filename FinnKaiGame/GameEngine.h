#pragma once

#include <memory>
#include <vector>
#include <SDL.h>

#include "Scene.h"
#include "TestScene.h"
#include "EntityManager.h"

class TestScene;

class GameEngine
{
public:

  const float WINDOW_WIDTH = 1960.f;
  const float WINDOW_HEIGHT = 1080.f;
  static GameEngine* instance() { 
    if (_instance == nullptr)
      _instance = std::unique_ptr<GameEngine>(new GameEngine);
    return _instance.get();
  }

  void run();
  void quit();


  SDL_Window* window() { return m_window; };
  EntityManager* entityManager() { return m_entityManager.get(); };
  bool isRunning();

private:
  static std::unique_ptr<GameEngine> _instance;

  GameEngine() : m_running(true) { init(); }

  SDL_Window* m_window;
  SDL_Renderer* m_renderer;
  std::unique_ptr<EntityManager> m_entityManager;
  std::unique_ptr<TestScene> m_scene;
 

  bool m_running;
  void init();
  void update();

 
};