#include "Game4.h"

#include <DxLib.h>

#include "../Peripheral.h"
#include "../Button.h"

#include "../Scene/SceneManager.h"
#include "../Scene/ResultScene.h"
#include "../Scene/PauseScene.h"


constexpr int qMax = 20;	// 問題の最大数

void Game4::FadeinUpdate(const Peripheral & p)
{
	if (pal > 255)
	{
		if (p.IsTrigger(MOUSE_INPUT_LEFT))
		{
			pal = 255;
			updater = &Game4::DescriptionUpdate;
			drawer = &Game4::DescriptionDraw;
		}
	}
	else
	{
		pal += 20;
	}
}

void Game4::FadeoutUpdate(const Peripheral & p)
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

void Game4::WaitUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		pal = 255;
		updater = &Game4::FadeoutUpdate;
	}
	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}
}

void Game4::DescriptionUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		updater = &Game4::QuestionDisplayUpdate;
		drawer = &Game4::GameDraw;
	}
}

void Game4::QuestionDisplayUpdate(const Peripheral & p)
{
	for (unsigned int i = 0; i < buttons.size(); ++i)
	{
		if (buttons[i]->Update(p))
		{
			updater = &Game4::AnswerCheckUpdate;
		}
	}

	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}
}

void Game4::AnswerCheckUpdate(const Peripheral & p)
{
	updater = &Game4::AnswerDisplayUpdate;
}

void Game4::AnswerDisplayUpdate(const Peripheral & p)
{
	if (displayCount <= 0)
	{
		if (nowQNum >= qMax)
		{
			updater = &Game4::FadeoutUpdate;
		}
		else
		{
			CreateQuestion();
			++nowQNum;

			updater = &Game4::QuestionDisplayUpdate;
		}
	}
	--displayCount;
}

void Game4::TitleDraw()
{
	DxLib::DrawString(400, 50, "ひらがな計算だよ", 0xffffff);
}

void Game4::DescriptionDraw()
{
	DxLib::DrawString(400, 50, "ルール説明", 0xffffff);
}

void Game4::GameDraw()
{
	DxLib::DrawFormatString(600, 600, 0xff0000, "第%d問", nowQNum);

	for (unsigned int i = 0; i < buttons.size(); ++i)
	{
		buttons[i]->Draw();
	}

}

void Game4::CreateQuestion()
{
}

Game4::Game4()
{
	updater = &Game4::FadeinUpdate;
	drawer = &Game4::TitleDraw;

	nowQNum = 1;
	displayCount = 60;

	Vector2 buttonSize = Vector2(100, 100);

	// テンキー
	for (int i = 0; i < 10; ++i)
	{
		buttons.emplace_back(new Button(Rect(100 + i % 3 * buttonSize.x, 100 + i / 3 * buttonSize.y, buttonSize.x, buttonSize.y)));
	}
	
}


Game4::~Game4()
{
}

void Game4::Update(const Peripheral & p)
{
	(this->*updater)(p);
}

void Game4::Draw()
{
	(this->*drawer)();
}
