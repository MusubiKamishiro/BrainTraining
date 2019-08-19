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
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		pal = 255;
		updater = &ResultScene::FadeoutUpdate;
	}
}

ResultScene::ResultScene(int questions, int correct)
{
	_questions = questions;
	_correct = correct;

	updater = &ResultScene::FadeinUpdate;
}


ResultScene::~ResultScene()
{
}

void ResultScene::Update(const Peripheral& p)
{
	(this->*updater)(p);
}

void ResultScene::Draw()
{
	DxLib::DrawBox(0, 0, 100, 100, 0xff00ff, true);
	DxLib::DrawString(450, 450, "リザルトシーンだよ", 0xffffff);

	DrawFormatString(450, 500, 0xffffff, "%d / %d", _correct, _questions);
}
