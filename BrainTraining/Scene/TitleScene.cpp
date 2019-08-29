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
		pal = 255;
		updater = &TitleScene::FadeoutUpdate;
	}
}

TitleScene::TitleScene()
{
	updater = &TitleScene::FadeinUpdate;

	ssize = Game::Instance().GetScreenSize();

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/img.png", data);
	img = data.GetHandle();

	_img_balance = LoadGraph("img/Title/balance.png");
	_img_flag = LoadGraph("img/Title/flag3.png");
	_img_math = LoadGraph("img/Title/math.png");
	_img_kokuban = LoadGraph("img/Title/kokuban.png");
}


TitleScene::~TitleScene()
{
	DeleteGraph(_img_balance);
	DeleteGraph(_img_flag);
	DeleteGraph(_img_math);
	DeleteGraph(_img_kokuban);
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
	DrawRotaGraph(size.x / 4, size.y / 2, 1.0, DX_PI_F / 180.0f * -30.0f, _img_balance, true);
	DrawRotaGraph(size.x / 4 * 3, size.y / 2, 1.0, DX_PI_F / 180.0f * 30.0f, _img_flag, true);
	DrawRotaGraph(size.x / 2, size.y / 2, 0.7, DX_PI_F / 180.0f * 0.0f, _img_math, true);

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(400);
	GetDrawStringSize(&strwidth, &strheight, nullptr, "”]ƒgƒŒ", strlen("”]ƒgƒŒ"));
	DxLib::DrawFormatString(size.x / 2 - strwidth / 2, size.y / 3 - strheight / 2, 0xffffff, "”]ƒgƒŒ");
}
