#include "Game1.h"

#include <DxLib.h>

#include "../Peripheral.h"

#include "../Scene/SceneManager.h"
#include "../Scene/ResultScene.h"
#include "../Scene/PauseScene.h"



void Game1::FadeinUpdate(const Peripheral & p)
{
	if (pal > 255)
	{
		pal = 255;
		updater = &Game1::WaitUpdate;
	}
	else
	{
		pal += 20;
	}
}

void Game1::FadeoutUpdate(const Peripheral & p)
{
	if (pal <= 0)
	{
		SceneManager::Instance().ChangeScene(std::make_unique<ResultScene>());
	}
	else
	{
		pal -= 20;
	}
}

void Game1::WaitUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		pal = 255;
		updater = &Game1::FadeoutUpdate;
	}
	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}
}

Game1::Game1()
{
	updater = &Game1::FadeinUpdate;
}


Game1::~Game1()
{
}

void Game1::Update(const Peripheral & p)
{
	(this->*updater)(p);
}

void Game1::Draw()
{
	DxLib::DrawBox(0, 0, 100, 100, 0x0000ff, true);
	DxLib::DrawString(450, 450, "ゲームシーン[1]だよ", 0xffffff);
}
