#include "Game6.h"

#include <DxLib.h>

#include "../Peripheral.h"

#include "../Scene/SceneManager.h"
#include "../Scene/ResultScene.h"
#include "../Scene/PauseScene.h"



void Game6::FadeinUpdate(const Peripheral & p)
{
	if (pal > 255)
	{
		pal = 255;
		updater = &Game6::WaitUpdate;
	}
	else
	{
		pal += 20;
	}
}

void Game6::FadeoutUpdate(const Peripheral & p)
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

void Game6::WaitUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		pal = 255;
		updater = &Game6::FadeoutUpdate;
	}
	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}
}

Game6::Game6()
{
	updater = &Game6::FadeinUpdate;
}


Game6::~Game6()
{
}

void Game6::Update(const Peripheral & p)
{
	(this->*updater)(p);
}

void Game6::Draw()
{
	DxLib::DrawBox(0, 0, 100, 100, 0x0000ff, true);
	DxLib::DrawString(450, 450, "ÉQÅ[ÉÄÉVÅ[Éì[6]ÇæÇÊ", 0xffffff);
}
