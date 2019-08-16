#include "Game1.h"

#include <DxLib.h>
#include <random>

#include "../Peripheral.h"
#include "../Game.h"
#include "../Button.h"
#include "../Geometry.h"

#include "../Scene/SceneManager.h"
#include "../Scene/ResultScene.h"
#include "../Scene/PauseScene.h"

#include "../System/FileSystem.h"
#include "../System/ImageLoader.h"

constexpr int qMax = 10;	// 問題の最大数


void Game1::FadeinUpdate(const Peripheral & p)
{
	if (pal > 255)
	{
		if (p.IsTrigger(MOUSE_INPUT_LEFT))
		{
			pal = 255;
			updater = &Game1::DescriptionUpdate;
			drawer = &Game1::DescriptionDraw;
		}
	}
	else
	{
		pal += 20;
	}
}

void Game1::FadeoutUpdate(const Peripheral & p)
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

void Game1::WaitUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}
}

void Game1::DescriptionUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		updater = &Game1::QuestionDisplayUpdate;
		drawer = &Game1::GameDraw;
	}
}

void Game1::QuestionDisplayUpdate(const Peripheral & p)
{
	for (unsigned int i = 0; i < buttons.size(); ++i)
	{
		if (buttons[i]->Update(p))
		{
			displayCount = 60;
			answerMyHand = static_cast<Hand>(i);
			updater = &Game1::AnswerCheckUpdate;
		}
	}

	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}
}

void Game1::AnswerCheckUpdate(const Peripheral & p)
{
	if (qHandNum == static_cast<int>(Hand::ROCK))
	{
		if (answerMyHand == Hand::ROCK)
		{
			if ((qStatementNum == static_cast<int>(Question::DONOTWIN)) || (qStatementNum == static_cast<int>(Question::DONOTLOSE)))
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
		else if (answerMyHand == Hand::SCISSORS)
		{
			if ((qStatementNum == static_cast<int>(Question::LOSE)) || (qStatementNum == static_cast<int>(Question::DONOTWIN)))
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
		else if (answerMyHand == Hand::PAPER)
		{
			if ((qStatementNum == static_cast<int>(Question::WIN)) || (qStatementNum == static_cast<int>(Question::DONOTLOSE)))
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
	}
	else if (qHandNum == static_cast<int>(Hand::SCISSORS))
	{
		if (answerMyHand == Hand::ROCK)
		{
			if ((qStatementNum == static_cast<int>(Question::WIN)) || (qStatementNum == static_cast<int>(Question::DONOTLOSE)))
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
		else if (answerMyHand == Hand::SCISSORS)
		{
			if ((qStatementNum == static_cast<int>(Question::DONOTWIN)) || (qStatementNum == static_cast<int>(Question::DONOTLOSE)))
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
		else if (answerMyHand == Hand::PAPER)
		{
			if ((qStatementNum == static_cast<int>(Question::LOSE)) || (qStatementNum == static_cast<int>(Question::DONOTWIN)))
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
	}
	else if (qHandNum == static_cast<int>(Hand::PAPER))
	{
		if (answerMyHand == Hand::ROCK)
		{
			if ((qStatementNum == static_cast<int>(Question::LOSE)) || (qStatementNum == static_cast<int>(Question::DONOTWIN)))
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
		else if (answerMyHand == Hand::SCISSORS)
		{
			if ((qStatementNum == static_cast<int>(Question::WIN)) || (qStatementNum == static_cast<int>(Question::DONOTLOSE)))
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
		else if (answerMyHand == Hand::PAPER)
		{
			if ((qStatementNum == static_cast<int>(Question::DONOTWIN)) || (qStatementNum == static_cast<int>(Question::DONOTLOSE)))
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
	}

	updater = &Game1::AnswerDisplayUpdate;
}

void Game1::AnswerDisplayUpdate(const Peripheral & p)
{
	if (displayCount <= 0)
	{
		if (nowQNum >= qMax)
		{
			updater = &Game1::FadeoutUpdate;
		}
		else
		{
			std::random_device seed_gen;
			std::mt19937 engine(seed_gen());

			qStatementNum = engine() % questionStatements.size();
			qHandNum = engine() % questionHands.size();
			++nowQNum;

			updater = &Game1::QuestionDisplayUpdate;
		}
	}
	--displayCount;
}

void Game1::TitleDraw()
{
	DxLib::DrawString(400, 50, "後出しじゃんけんだよ", 0xffffff);
}

void Game1::DescriptionDraw()
{
	DxLib::DrawString(400, 50, "ルール説明だよ", 0xffffff);
}

void Game1::GameDraw()
{
	DxLib::DrawFormatString(600, 600, 0xff0000, "第%d問", nowQNum);

	// グーちょきぱーボタンの描画
	for (unsigned int i = 0; i < buttons.size(); ++i)
	{
		buttons[i]->Draw();
	}

	// 問題文章の描画
	std::string s = questionStatements[qStatementNum];
	DxLib::DrawFormatString(700, 100, 0xffffff, "%s", s.c_str());

	// 問題の手
	DxLib::DrawExtendGraph(810, 150, 1110, 450, questionHands[qHandNum], true);

	if (updater == &Game1::AnswerDisplayUpdate)
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

Game1::Game1()
{
	updater = &Game1::FadeinUpdate;
	drawer = &Game1::TitleDraw;

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/rock.png", data);
	rock = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/scissors.png", data);
	scissors = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/paper.png", data);
	paper = data.GetHandle();
	
	buttons.emplace_back(new Button(Rect(455, 850, 300, 300), rock));
	buttons.emplace_back(new Button(Rect(955, 850, 300, 300), scissors));
	buttons.emplace_back(new Button(Rect(1455, 850, 300, 300), paper));

	questionHands[static_cast<int>(Hand::ROCK)] = rock;
	questionHands[static_cast<int>(Hand::SCISSORS)] = scissors;
	questionHands[static_cast<int>(Hand::PAPER)] = paper;
	qHandNum = 0;

	questionStatements[0] = "勝ってください";
	questionStatements[1] = "負けてください";
	questionStatements[2] = "勝たないでください";
	questionStatements[3] = "負けないでください";
	qStatementNum = 0;

	displayCount = 60;
	nowQNum = 1;

	result = false;
}

Game1::~Game1()
{
}

void Game1::Update(const Peripheral & p)
{
	(this->*updater)(p);
}

void Game1::Draw()
{
	DxLib::DrawString(50, 50, "後出しじゃんけんだよ", 0xffffff);
	(this->*drawer)();
}
