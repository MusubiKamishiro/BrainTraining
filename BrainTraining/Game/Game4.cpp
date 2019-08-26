#include "Game4.h"

#include <DxLib.h>
#include <random>
#include <string>

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
			displayCount = 60;

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
	DxLib::DrawFormatString(400, 400, 0xff0000, "%s", question.c_str());

	for (unsigned int i = 0; i < buttons.size(); ++i)
	{
		buttons[i]->Draw();
	}
}

int Game4::RandomNum(int parameter)
{
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());

	int num = engine() % parameter;

	return num;
}

std::string Game4::CreateHiraganaNum(int num)
{
	std::string s = "";

	// 1桁か2桁か
	if (num / 10 == 0)
	{
		s = hiraganaNum[num];
	}
	else
	{
		s = hiraganaNum[num / 10];
		s += hiraganaNum[10];

		if (num % 10 != 0)
		{
			s += hiraganaNum[num % 10];
		}
	}

	return s;
}

void Game4::CreateQuestion()
{
	question = "";

	SelectNum(100);
	SelectOperator();
	if (op == static_cast<int>(Operator::PLUS))
	{
		SelectNum(100);
	}
	else if (op == static_cast<int>(Operator::MINUS))
	{
		SelectNum(100);
	}
	else if (op == static_cast<int>(Operator::MULTI))
	{
		SelectNum(10);
	}
	

	question += "は？";
}

void Game4::SelectNum(int num)
{
	int n = RandomNum(num);
	std::string s = CreateHiraganaNum(n);

	question += s;
}

void Game4::SelectOperator()
{
	if (nowQNum > qMax / 2)
	{
		op = RandomNum(static_cast<int>(Operator::MAX));
	}
	else
	{
		op = RandomNum(static_cast<int>(Operator::MAX) - 1);
	}

	question += questionOperators[op];
}

Game4::Game4()
{
	updater = &Game4::FadeinUpdate;
	drawer = &Game4::TitleDraw;

	questionOperators[static_cast<int>(Operator::PLUS)] = "たす";
	questionOperators[static_cast<int>(Operator::MINUS)] = "ひく";
	questionOperators[static_cast<int>(Operator::MULTI)] = "かけ";

	hiraganaNum[0] = "ぜろ";
	hiraganaNum[1] = "いち";
	hiraganaNum[2] = "に";
	hiraganaNum[3] = "さん";
	hiraganaNum[4] = "よん";
	hiraganaNum[5] = "ご";
	hiraganaNum[6] = "ろく";
	hiraganaNum[7] = "なな";
	hiraganaNum[8] = "はち";
	hiraganaNum[9] = "きゅう";
	hiraganaNum[10] = "じゅう";


	CreateQuestion();

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
