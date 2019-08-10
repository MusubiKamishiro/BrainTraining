#include "SceneManager.h"
#include <DxLib.h>
#include "Scene.h"
#include "TitleScene.h"
#include "../Game.h"



SceneManager::SceneManager()
{
	scene.emplace_front(std::make_unique<TitleScene>());
}


SceneManager::~SceneManager()
{
}

void SceneManager::ChangeScene(std::unique_ptr<Scene> newScene)
{
	while (scene.size())
	{
		scene.pop_front();
	}
	scene.emplace_front(std::move(newScene));
}

void SceneManager::PushScene(std::unique_ptr<Scene> newScene)
{
	scene.emplace_front(std::move(newScene));
}

void SceneManager::PopScene()
{
	scene.pop_front();
}

void SceneManager::Update(Peripheral & p)
{
	scene.front()->Update(p);
}

void SceneManager::Draw()
{
	for (int i = (scene.size() - 1); i >= 0; --i)
	{
		auto pal = scene[i]->GetPal();
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);

		scene[i]->Draw();

		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::abs(pal - 255));
		DxLib::DrawBox(0, 0, Game::Instance().GetScreenSize().x, Game::Instance().GetScreenSize().y, 0x000000, true);
	}
}
