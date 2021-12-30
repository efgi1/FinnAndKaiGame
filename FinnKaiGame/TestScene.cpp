#include "TestScene.h"
#include "TestRenderSystem.h"
#include "TestInputSystem.h"
#include "TransformComponent.h"
#include "TestMovementSystem.h"
#include "TestAnimationSystem.h"

TestScene::TestScene() : Scene()
{
  addSystem(std::make_unique<TestSystem>());
  addSystem(std::make_unique< TestRenderSystem>());
  addSystem(std::make_unique< TestInputSystem>());
  addSystem(std::make_unique<TestMovementSystem>());

  auto entity = GameEngine::instance()->entityManager()->addEntity("test");
  entity->addComponent<CTransform>(glm::vec2(0,0));
  entity->getComponent<CTransform>().velocity = glm::vec2(1, 1);
  entity->addComponent<CAnimation>(GameEngine::instance()->assetManager()->getAnimation("kai"), false);

}
