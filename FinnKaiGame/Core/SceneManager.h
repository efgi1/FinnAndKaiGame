#pragma once
#include "..\Common\Scene.h"
#include <memory>

class SceneManager
{
public:
	SceneManager() : m_scene(std::make_unique<Scene>()) {}

	void setCurrentScene(std::unique_ptr<Scene> scene) { m_scene = std::move(scene); }
	void update() { m_scene->update(); }
private:
	std::unique_ptr<Scene> m_scene;
};

