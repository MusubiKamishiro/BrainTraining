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
		PlaySoundMem(_se, DX_PLAYTYPE_BACK);
		pal = 255;
		updater = &TitleScene::FadeoutUpdate;
	}
}

TitleScene::TitleScene()
{
	updater = &TitleScene::FadeinUpdate;

	_img_flag = LoadGraph("img/Title/flag3.png");
	_img_kokuban = LoadGraph("img/Title/kokuban.png");

	_bgm = LoadSoundMem("BGM/title.mp3");
	_se = LoadSoundMem("SE/decide.mp3");
	_blindCnt = 0;
	PlaySoundMem(_bgm, DX_PLAYTYPE_BACK);
}


TitleScene::~TitleScene()
{
	DeleteGraph(_img_flag);
	DeleteGraph(_img_kokuban);
	DeleteSoundMem(_bgm);
	DeleteSoundMem(_se);
}

void TitleScene::Update(const Peripheral& p)
{
	++_blindCnt;
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
	GetDrawStringSize(&strwidth, &strheight, nullptr, "世界一", strlen("世界一"));
	DxLib::DrawFormatString(size.x / 5 + 5, size.y / 4 + 5, 0x000000, "世界一");
	DxLib::DrawFormatString(size.x / 5, size.y / 4, 0xffffff, "世界一");
	GetDrawStringSize(&strwidth, &strheight, nullptr, "受けたい", strlen("受けたい"));
	DxLib::DrawFormatString(size.x / 5 + 5, size.y / 2 + 5, 0x000000, "受けたい");
	DxLib::DrawFormatString(size.x / 5, size.y / 2, 0xffffff, "受けたい");
	DxLib::DrawFormatString(size.x / 5 + strwidth + 5, size.y / 2 + 5, 0x000000, "脳トレ");
	DxLib::DrawFormatString(size.x / 5 + strwidth, size.y / 2, 0xff2222, "脳トレ");

	if (!((_blindCnt / 40) % 2))
	{
		SetFontSize(120);
		GetDrawStringSize(&strwidth, &strheight, nullptr, "画面をクリックしてね!", strlen("画面をクリックしてね!"));
		DrawString(size.x / 2 - strwidth / 2, strheight / 3, "画面をクリックしてね!", 0xdddd00);
	}
}
