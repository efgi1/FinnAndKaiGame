#include "TestScene.h"
#include "TestRenderSystem.h"
#include "TestInputSystem.h"
#include "TransformComponent.h"
#include "InputComponent.h"
#include "GravityComponent.h"
#include "TestMovementSystem.h"
#include "TestAnimationSystem.h"
#include "AnimationComponent.h"

TestScene::TestScene() : Scene()
{
	addSystem<TestSystem>();
	addSystem<TestInputSystem>();
	addSystem<TestRenderSystem>();
	addSystem<TestMovementSystem>();

	auto em = GameEngine::instance()->entityManager();
	m_player = em->create();
	em->emplace<std::string>(m_player, "player");
	em->emplace<CTransform>(m_player, glm::vec2(0,0));
	em->emplace<CAnimation>(m_player, "kai", *(GameEngine::instance()->assetManager()->getAnimation("kai_standing")), false);
	em->emplace<CInput>(m_player);
	em->emplace<CGravity>(m_player, kGravity);
}
