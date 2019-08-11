#include "SelectScene.h"
#include <DxLib.h>
#include "../Peripheral.h"
#include "../Game.h"
#include "SceneManager.h"
#include "GamePlayingScene.h"


void SelectScene::FadeinUpdate(const Peripheral & p)
{
	if (pal >= 255)
	{
		pal = 255;
		updater = &SelectScene::WaitUpdate;
	}
	else
	{
		pal += 20;
	}
}

void SelectScene::FadeoutUpdate(const Peripheral & p)
{
	if (pal <= 0)
	{
		SceneManager::Instance().ChangeScene(std::make_unique <GamePlayingScene>());
	}
	else
	{
		pal -= 20;
	}
}

void SelectScene::WaitUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		updater = &SelectScene::FadeoutUpdate;
	}
}

SelectScene::SelectScene()
{
	updater = &SelectScene::FadeinUpdate;
}


SelectScene::~SelectScene()
{
}

void SelectScene::Update(const Peripheral& p)
{
	(this->*updater)(p);
}

void SelectScene::Draw()
{
	DxLib::DrawBox(0, 0, 100, 100, 0x00ff00, true);
}
