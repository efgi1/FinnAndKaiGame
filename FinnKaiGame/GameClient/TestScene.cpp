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
}
