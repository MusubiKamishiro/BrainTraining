#include "ResultScene.h"
#include <DxLib.h>
#include <string>
#include "../Peripheral.h"
#include "../Game.h"
#include "SceneManager.h"
#include "TitleScene.h"


void ResultScene::FadeinUpdate(const Peripheral & p)
{
	if (pal >= 255)
	{
		pal = 255;
		updater = &ResultScene::WaitUpdate;
	}
	else
	{
		pal +=20;
	}
}

void ResultScene::FadeoutUpdate(const Peripheral & p)
{
	if (pal <= 0)
	{
		SceneManager::Instance().ChangeScene(std::make_unique <TitleScene>());
	}
	else
	{
		pal -= 20;
	}
}

void ResultScene::WaitUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT) && _frame > 60)
	{
		pal = 255;
		updater = &ResultScene::FadeoutUpdate;
	}
}

ResultScene::ResultScene(int questions, int correct)
{
	_questions = questions;
	_correct = correct;

	_frame = 0;

	_img_back = LoadGraph("img/Result/note.png");
	_img_rankS = LoadGraph("img/Result/rank_s.png");
	_img_rankA = LoadGraph("img/Result/rank_a.png");
	_img_rankB = LoadGraph("img/Result/rank_b.png");
	_img_rankC = LoadGraph("img/Result/rank_c.png");
	_img_rankD = LoadGraph("img/Result/rank_d.png");

	updater = &ResultScene::FadeinUpdate;
}


ResultScene::~ResultScene()
{
	DeleteGraph(_img_back);
	DeleteGraph(_img_rankS);
	DeleteGraph(_img_rankA);
	DeleteGraph(_img_rankB);
	DeleteGraph(_img_rankC);
	DeleteGraph(_img_rankD);
}

void ResultScene::Update(const Peripheral& p)
{
	(this->*updater)(p);
}

void ResultScene::Draw()
{
	auto size = Game::Instance().GetScreenSize();

	DrawRotaGraph(size.x / 2, size.y / 2, 1.6, 0.0, _img_back, true);

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(500);
	GetDrawStringSize(&strwidth, &strheight, nullptr, "/", strlen("/"));
	DrawString(size.x / 3 - strwidth / 2, size.y / 2 - strheight / 2, "/", 0xff2222);
	SetFontSize(300);
	DrawFormatString(size.x / 3, size.y / 2, 0xff2222, "%d", _questions);

	if (_frame > 25)
	{
		SetFontSize(600);
		DrawFormatString(size.x / 8, size.y / 8, 0xff2222, "%d", _correct);
	}
	if (_frame > 35)
	{
		float rate = (float)_correct / _questions;

		if (rate >= 1.0f)
		{
			DrawRotaGraph(size.x / 4 * 3, size.y / 2, 1.0, DX_PI_F / 180.0f * 30.0f, _img_rankS, true);
		}
		else if (rate >= 0.8f)
		{
			DrawRotaGraph(size.x / 4 * 3, size.y / 2, 1.0, DX_PI_F / 180.0f * 30.0f, _img_rankA, true);
		}
		else if (rate >= 0.6f)
		{
			DrawRotaGraph(size.x / 4 * 3, size.y / 2, 1.0, DX_PI_F / 180.0f * 30.0f, _img_rankB, true);
		}
		else if (rate >= 0.4f)
		{
			DrawRotaGraph(size.x / 4 * 3, size.y / 2, 1.0, DX_PI_F / 180.0f * 30.0f, _img_rankC, true);
		}
		else
		{
			DrawRotaGraph(size.x / 4 * 3, size.y / 2, 1.0, DX_PI_F / 180.0f * 30.0f, _img_rankD, true);
		}
	}

	++_frame;
}
