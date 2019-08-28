#include "Game4.h"

#include <DxLib.h>
#include <random>
#include <string>

#include "../Peripheral.h"
#include "../Button.h"
#include "../Game.h"

#include "../Scene/SceneManager.h"
#include "../Scene/ResultScene.h"
#include "../Scene/PauseScene.h"

#include "../System/FileSystem.h"
#include "../System/SoundLoader.h"

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
		SceneManager::Instance().ChangeScene(std::make_unique<ResultScene>(qMax, trueNum));
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

	if (result)
	{
		++trueNum;
		DxLib::PlaySoundMem(trueSE, DX_PLAYTYPE_BACK, true);
	}
	else
	{
		DxLib::PlaySoundMem(falseSE, DX_PLAYTYPE_BACK, true);
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
	auto size = Game::Instance().GetScreenSize();

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(250);
	std::string s = "ひらがな計算";
	GetDrawStringSize(&strwidth, &strheight, nullptr, s.c_str(), strlen(s.c_str()));
	DrawString(size.x / 2 - strwidth / 2, size.y / 3 - strheight / 2, s.c_str(), 0x000000);

	SetFontSize(100);
	GetDrawFormatStringSize(&strwidth, &strheight, nullptr, "全%d問", qMax);
	DrawFormatString(size.x / 2 - strwidth / 2, size.y / 3 * 2 - strheight / 2, 0x000000, "全%d問", qMax);
}

void Game4::DescriptionDraw()
{
	DxLib::DrawString(400, 50, "ルール説明", 0x000000);
}

void Game4::GameDraw()
{
	DxLib::SetFontSize(96);
	DxLib::DrawFormatString(200, 200, 0x000000, "第%d問", nowQNum);
	DxLib::DrawFormatString(50, 400, 0x000000, "%s", question.c_str());

	DxLib::DrawFormatString(500, 800, 0x000000, "自分の回答:%d", myAnswer);

	// 各種ボタンの描画
	for (unsigned int i = 0; i < buttons.size(); ++i)
	{
		buttons[i]->Draw();
		auto rect = buttons[i]->GetButtonRect();
		DxLib::DrawFormatString(rect.center.x, rect.center.y, 0x000000, "%d", i);
	}
	decide->Draw();
	auto rect = decide->GetButtonRect();
	DxLib::DrawFormatString(rect.center.x - rect.Width() / 4, rect.center.y, 0x000000, "%s", "回答する");
	del->Draw();
	rect = del->GetButtonRect();
	DxLib::DrawFormatString(rect.center.x - rect.Width() / 4, rect.center.y, 0x000000, "%s", "取り消し");

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
	
	if (myAnswer > maxQNun * maxQNun)
	{
		myAnswer = 0;
	}

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
		if (num / 10 != 1)
		{
			s = hiraganaNum[num / 10];
		}
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
	// 問題文と自分の回答を初期化
	myAnswer = 0;
	question = "";

	// まず、何算になるかを決める
	SelectOperator();

	// 1つめの数値を決定
	if (nowQNum < qMax / 4)
	{
		SelectNum(maxQNun / 10, firstNum);
	}
	else
	{
		SelectNum(maxQNun, firstNum);
	}
	
	// 問題文章に演算子の追加
	question += questionOperators[op];
	question += " ";

	// 演算子をみて、2つめの数値を決定
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
	question += " ";
}

void Game4::SelectOperator()
{
	if (nowQNum > qMax / 2)
	{
		op = RandomNum(static_cast<int>(Operator::MAX));
	}
	else
	{
		op = RandomNum(static_cast<int>(Operator::MULTI));
	}
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
	trueNum = 0;

	nowQNum = 1;
	displayCount = 60;
	result = false;

	SoundData sData;
	Game::Instance().GetFileSystem()->Load("SE/correct1.mp3", sData);
	trueSE = sData.GetHandle();
	Game::Instance().GetFileSystem()->Load("SE/incorrect1.mp3", sData);
	falseSE = sData.GetHandle();

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

	CreateQuestion();
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
	auto size = Game::Instance().GetScreenSize();
	DxLib::DrawBox(0, 0, size.x, size.y, 0xffffff, true);
	(this->*drawer)();
}
