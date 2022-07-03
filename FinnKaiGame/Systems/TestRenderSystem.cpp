#include "TestRenderSystem.h"
#include "..\imgui\imgui.h"
#include "..\imgui\imgui_impl_sdl.h"
#include "..\imgui\imgui_impl_sdlrenderer.h"
#include "..\imgui\imgui_internal.h"

void TestRenderSystem::operator()()
{
	auto window = GameEngine::instance()->window();
	auto surface = SDL_GetWindowSurface(window);
	auto renderer = GameEngine::instance()->renderer();
	if (window == nullptr || renderer == nullptr)
		return;

	SDL_RenderClear(renderer);

	auto entities = GameEngine::instance()->entityManager()->getEntities();

	for (auto& entity : entities)
	{
		auto& animation = entity->getComponent<CAnimation>();
		auto transform = entity->getComponent<CTransform>();
		if (animation.implemented && transform.implemented)
		{
			auto spriteRect = animation.animation.getSprite();
			auto texture = GameEngine::instance()->assetManager()->getTexture(animation.textureName)->getSDLTexture();
			auto renderer = GameEngine::instance()->renderer();
			SDL_Rect renderRect;
			renderRect.w = static_cast<int>(spriteRect.w * transform.scale.x);
			renderRect.h = static_cast<int>(spriteRect.h * transform.scale.y);
			renderRect.x = static_cast<int>(transform.pos.x);
			renderRect.y = static_cast<int>(transform.pos.y);
			
			SDL_RenderCopyEx(renderer, texture, &spriteRect, &renderRect, 0, nullptr, animation.animation.getFlip());
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x0FF, 0xFF);
			entity->getComponent<CAnimation>().animation.update();
		}
	}

	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	bool yes = true;
	ImGui::ShowDemoWindow(&yes);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			//ImGui::MenuItem("(demo menu)", NULL, false, false);
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..")) {}
			if (ImGui::MenuItem("Exit"))
			{
				GameEngine::instance()->quit();
				return;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		auto buttonWidth = ImGui::CalcTextSize("X").x + ImGui::GetStyle().ItemInnerSpacing.x * 2;
		auto frameWidth = ImGui::GetStyle().FramePadding.x;
		if (ImGui::CloseButton(1, ImVec2(GameEngine::instance()->WINDOW_WIDTH - buttonWidth - frameWidth, 0)))
		{
			GameEngine::instance()->quit();
			return;
		};
		ImGui::EndMainMenuBar();
	}

	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(renderer);
	
}
