
#include "GameEngine.h"

#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
  GameEngine::instance()->run();
  return 0;
}
