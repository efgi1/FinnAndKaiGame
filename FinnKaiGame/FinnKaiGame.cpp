
#include "GameEngine.h"

#include <iostream>

std::unique_ptr<GameEngine> GameEngine::_instance = nullptr;

int main(int argc, char* argv[])
{
  GameEngine::instance()->run();
  return 0;
}
