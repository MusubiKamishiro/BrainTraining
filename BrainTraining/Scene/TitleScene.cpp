#include "TitleScene.h"
#include <DxLib.h>
#include "../Peripheral.h"
#include "../Game.h"
#include "SceneManager.h"
#include "SelectScene.h"

#include "../System/FileSystem.h"
#include "../System/ImageLoader.h"


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
		StopSoundMem(_bgm);
		pal = 255;
		updater = &TitleScene::FadeoutUpdate;
	}
}

TitleScene::TitleScene()
{
	updater = &TitleScene::FadeinUpdate;

	ssize = Game::Instance().GetScreenSize();

	_img_flag = LoadGraph("img/Title/flag3.png");
	_img_kokuban = LoadGraph("img/Title/kokuban.png");

	_bgm = LoadSoundMem("BGM/title.mp3");
	PlaySoundMem(_bgm, DX_PLAYTYPE_BACK);
}


TitleScene::~TitleScene()
{
	DeleteGraph(_img_flag);
	DeleteGraph(_img_kokuban);
	DeleteSoundMem(_bgm);
}

void TitleScene::Update(const Peripheral& p)
{
	(this->*updater)(p);
}

void TitleScene::Draw()
{
	auto size = Game::Instance().GetScreenSize();

	DrawBox(0, 0, size.x, size.y, 0xffffff, true);
	DrawRotaGraph(size.x / 2, size.y / 2, 1.3, 0.0, _img_kokuban, true);
	DrawRotaGraph(size.x / 3 * 2, size.y / 2, 1.1, DX_PI_F / 180.0f * 20.0f, _img_flag, true);

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(200);
	GetDrawStringSize(&strwidth, &strheight, nullptr, "ê¢äEàÍ", strlen("ê¢äEàÍ"));
	DxLib::DrawFormatString(size.x / 5 + 5, size.y / 4 + 5, 0x000000, "ê¢äEàÍ");
	DxLib::DrawFormatString(size.x / 5, size.y / 4, 0xffffff, "ê¢äEàÍ");
	GetDrawStringSize(&strwidth, &strheight, nullptr, "éÛÇØÇΩÇ¢", strlen("éÛÇØÇΩÇ¢"));
	DxLib::DrawFormatString(size.x / 5 + 5, size.y / 2 + 5, 0x000000, "éÛÇØÇΩÇ¢");
	DxLib::DrawFormatString(size.x / 5, size.y / 2, 0xffffff, "éÛÇØÇΩÇ¢");
	DxLib::DrawFormatString(size.x / 5 + strwidth + 5, size.y / 2 + 5, 0x000000, "î]ÉgÉå");
	DxLib::DrawFormatString(size.x / 5 + strwidth, size.y / 2, 0xff2222, "î]ÉgÉå");
}
