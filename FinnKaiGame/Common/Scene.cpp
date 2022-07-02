#include "..\Common\Scene.h"

void Scene::update()
{
    for (auto& system : m_systems)
    {
        if (!m_paused || !system->pausable)
            (*system)();
    }
}