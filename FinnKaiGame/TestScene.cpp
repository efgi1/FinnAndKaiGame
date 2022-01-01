#include "TestScene.h"
#include "TestRenderSystem.h"
#include "TestInputSystem.h"
#include "TransformComponent.h"
#include "InputComponent.h"
#include "GravityComponent.h"
#include "TestMovementSystem.h"
#include "TestAnimationSystem.h"

TestScene::TestScene() : Scene()
{
	addSystem<TestSystem>();
	addSystem<TestInputSystem>();
	addSystem<TestRenderSystem>();
	addSystem<TestMovementSystem>();

	m_player = GameEngine::instance()->entityManager()->addEntity("player");
	m_player->addComponent<CTransform>(glm::vec2(0,0));
	m_player->addComponent<CAnimation>(*(GameEngine::instance()->assetManager()->getAnimation("kai_standing")), false);
	m_player->addComponent<CInput>();
	m_player->addComponent<CGravity>(1.f);
}
