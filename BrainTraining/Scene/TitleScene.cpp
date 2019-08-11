#include "TitleScene.h"
#include <DxLib.h>
#include "../Peripheral.h"
#include "../Game.h"
#include "SceneManager.h"
#include "SelectScene.h"


void TitleScene::FadeinUpdate(const Peripheral & p)
{
	if (pal >= 255)
	{
		pal = 255;
		updater = &TitleScene::WaitUpdate;
	}
	else
	{
		pal += 20;
	}
}

void TitleScene::FadeoutUpdate(const Peripheral & p)
{
	if (pal <= 0)
	{
		SceneManager::Instance().ChangeScene(std::make_unique <SelectScene>());
	}
	else
	{
		pal -= 20;
	}
}

void TitleScene::WaitUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		pal = 255;
		updater = &TitleScene::FadeoutUpdate;
	}
}

TitleScene::TitleScene()
{
	updater = &TitleScene::FadeinUpdate;

	ssize = Game::Instance().GetScreenSize();
}


TitleScene::~TitleScene()
{
}

void TitleScene::Update(const Peripheral& p)
{
	(this->*updater)(p);
}

void TitleScene::Draw()
{
	DxLib::DrawBox(0, 0, 100, 100, 0xff0000, true);
	DxLib::DrawString(450, 450, "タイトルシーンだよ", 0xffffff);
}
