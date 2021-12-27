#include "TestScene.h"
#include "TestDrawSystem.h"
#include "TestInputSystem.h"
#include "TransformComponent.h"
#include "TestMovementSystem.h"

TestScene::TestScene() : Scene()
{
  addSystem(std::make_unique<TestSystem>());
  addSystem(std::make_unique< TestDrawSystem>());
  addSystem(std::make_unique< TestInputSystem>());
  addSystem(std::make_unique<TestMovementSystem>());

  auto entity = GameEngine::instance()->entityManager()->addEntity("test");
  entity->addComponent<CTransform>(glm::vec2(0,0));
  entity->getComponent<CTransform>().velocity = glm::vec2(1, 1);

  auto entity1 = GameEngine::instance()->entityManager()->addEntity("test");
  entity1->addComponent<CTransform>(glm::vec2(100, 100));
  entity1->getComponent<CTransform>().velocity = glm::vec2(1, 1);
  for (int i = 0; i < 50; ++i)
  {
    auto e = GameEngine::instance()->entityManager()->addEntity("random");
    e->addComponent<CTransform>(glm::vec2(rand() % 1960, rand() % 1080));
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    e->getComponent<CTransform>().velocity = glm::vec2(r, r2);
  }
}
