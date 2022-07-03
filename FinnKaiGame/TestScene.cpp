#include "TestScene.h"
#include "Systems\TestRenderSystem.h"
#include "Systems\TestInputSystem.h"
#include "Components\TransformComponent.h"
#include "Components\InputComponent.h"
#include "Components\GravityComponent.h"
#include "Systems\TestMovementSystem.h"
#include "Systems\TestAnimationSystem.h"

TestScene::TestScene() : Scene()
{
	addSystem<TestSystem>();
	addSystem<TestInputSystem>();
	addSystem<TestRenderSystem>();
	addSystem<TestMovementSystem>();

	m_player = GameEngine::instance()->entityManager()->addEntity("player");
	m_player->addComponent<CTransform>(glm::vec2(0,0));
	m_player->addComponent<CAnimation>("kai", * (GameEngine::instance()->assetManager()->getAnimation("kai_standing")), false);
	m_player->addComponent<CInput>();
	m_player->addComponent<CGravity>(0.f);
}
