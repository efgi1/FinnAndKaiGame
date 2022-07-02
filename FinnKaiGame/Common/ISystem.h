#pragma once
#include "..\Core\EntityManager.h"
#include <functional>
#include "..\Common\Scene.h"

class Scene;

class ISystem {
public:
	ISystem(Scene* scene) : m_scene(scene), pausable(false) {}
	Scene* m_scene;
	bool pausable;
	virtual void operator()() = 0;
};