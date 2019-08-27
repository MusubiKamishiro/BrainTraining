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
constexpr int maxQNun = 100;	// 最大値

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
			EditMyAnswer(i);
		}
	}
	if (decide->Update(p))
	{
		updater = &Game4::AnswerCheckUpdate;
	}
	if (del->Update(p))
	{
		myAnswer = 0;
	}

	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}
}

void Game4::AnswerCheckUpdate(const Peripheral & p)
{
	// 問題の回答を確認
	if (op == static_cast<int>(Operator::PLUS))
	{
		qAnswer = firstNum + secondNum;
	}
	else if (op == static_cast<int>(Operator::MINUS))
	{
		qAnswer = firstNum - secondNum;
	}
	else if (op == static_cast<int>(Operator::MULTI))
	{
		qAnswer = firstNum * secondNum;
	}

	if (qAnswer == myAnswer)
	{
		result = true;
	}
	else
	{
		result = false;
	}

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
		auto rect = buttons[i]->GetButtonRect();
		DxLib::DrawFormatString(rect.center.x, rect.center.y, 0xff0000, "%d", i);
	}
	decide->Draw();
	del->Draw();

	if (updater == &Game4::AnswerDisplayUpdate)
	{
		if (result)
		{
			DxLib::DrawCircle(600, 600, 100, 0x00ff00);
		}
		else
		{
			DxLib::DrawBox(450, 450, 550, 550, 0x00ff00, true);
		}
	}
}

void Game4::EditMyAnswer(unsigned int num)
{
	myAnswer *= 10;
	myAnswer += num;
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
	myAnswer = 0;

	question = "";

	SelectNum(maxQNun, firstNum);
	SelectOperator();
	if (op == static_cast<int>(Operator::PLUS))
	{
		SelectNum(maxQNun, secondNum);
	}
	else if (op == static_cast<int>(Operator::MINUS))
	{
		SelectNum(firstNum, secondNum);
	}
	else if (op == static_cast<int>(Operator::MULTI))
	{
		SelectNum(maxQNun/10, secondNum);
	}
	
	question += "は？";
}

void Game4::SelectNum(int num, int& qnum)
{
	if (num != 0)
	{
		qnum = RandomNum(num);
	}
	else
	{
		qnum = 0;
	}
	
	std::string s = CreateHiraganaNum(qnum);

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

	firstNum = secondNum = 0;
	qAnswer = myAnswer = 0;

	nowQNum = 1;
	displayCount = 60;
	result = false;

	CreateQuestion();


	// テンキー
	Vector2 buttonSize = Vector2(200, 200);
	buttons.emplace_back(new Button(Rect(1420, 700, buttonSize.x, buttonSize.y)));
	for (int i = 0; i < 9; ++i)
	{
		buttons.emplace_back(new Button(Rect(1420 + i % 3 * buttonSize.x, 500 - i / 3 * buttonSize.y, buttonSize.x, buttonSize.y)));
	}
	// 決定,取り消しボタン
	decide.reset(new Button(Rect(1620, 950, buttonSize.x * 3, buttonSize.y)));
	del.reset(new Button(Rect(1720, 700, buttonSize.x * 2, buttonSize.y)));
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
	DxLib::DrawFormatString(800, 600, 0xff0000, "回答:%d", qAnswer);
	DxLib::DrawFormatString(800, 800, 0xff0000, "自分の回答:%d", myAnswer);

	(this->*drawer)();
}
