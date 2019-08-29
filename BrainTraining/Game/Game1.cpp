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
#include "../System/SoundLoader.h"

constexpr int qMax = 20;	// 問題の最大数

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
		SceneManager::Instance().ChangeScene(std::make_unique<ResultScene>(qMax, trueNum));
	}
	else
	{
		pal -= 20;
	}
}

void Game1::WaitUpdate(const Peripheral & p)
{
	if (waitTime <= 0 && !CheckSoundMem(_startSE))
	{
		updater = &Game1::QuestionDisplayUpdate;
	}

	--waitTime;
	if (!(waitTime % 60) && waitTime > 0)
	{
		if (waitTime / 60 == 1)
		{
			PlaySoundMem(_startSE, DX_PLAYTYPE_BACK);
		}
		else
		{
			PlaySoundMem(_cntDownSE, DX_PLAYTYPE_BACK);
		}
	}
}

void Game1::DescriptionUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		PlaySoundMem(_cntDownSE, DX_PLAYTYPE_BACK);
		updater = &Game1::WaitUpdate;
		drawer = &Game1::GameDraw;
	}
	++count;
}

void Game1::QuestionDisplayUpdate(const Peripheral & p)
{
	if (!CheckSoundMem(_gameBGM))
	{
		PlaySoundMem(_gameBGM, DX_PLAYTYPE_BACK);
	}
	for (unsigned int i = 0; i < buttons.size(); ++i)
	{
		if (buttons[i]->Update(p))
		{
			displayCount = 30;
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
	Result r = JudgeResult(qHandNum, answerMyHand);

	if (r == Result::WIN)
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
	else if (r == Result::DRAW)
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
	else if(r == Result::LOSE)
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
			CreateQuestion();
			++nowQNum;

			updater = &Game1::QuestionDisplayUpdate;
		}
	}
	--displayCount;
}

void Game1::TitleDraw()
{
	auto size = Game::Instance().GetScreenSize();

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(230);
	std::string s = "後出しじゃんけん";
	GetDrawStringSize(&strwidth, &strheight, nullptr, s.c_str(), strlen(s.c_str()));
	DrawString(size.x / 2 - strwidth / 2, size.y / 3 - strheight / 2, s.c_str(), 0x000000);

	SetFontSize(100);
	GetDrawFormatStringSize(&strwidth, &strheight, nullptr, "全%d問", qMax);
	DrawFormatString(size.x / 2 - strwidth / 2, size.y / 3 * 2 - strheight / 2, 0x000000, "全%d問", qMax);
}

void Game1::DescriptionDraw()
{
	// グーちょきぱーボタンの描画
	for (unsigned int i = 0; i < buttons.size(); ++i)
	{
		buttons[i]->Draw();

		auto rect = buttons[i]->GetButtonRect();
		DxLib::DrawExtendGraph(rect.center.x - rect.Width() / 3, rect.center.y - rect.Height() / 3,
			rect.center.x + rect.Width() / 3, rect.center.y + rect.Height() / 3, handImg[i], true);
	}

	// 問題文章の描画
	DxLib::SetFontSize(96);
	std::string s = questionStatements[0];
	int strwidth, strheight;
	GetDrawStringSize(&strwidth, &strheight, nullptr, s.c_str(), strlen(s.c_str()));
	auto size = Game::Instance().GetScreenSize();
	DxLib::DrawFormatString(size.x / 2 - strwidth / 2, 70, 0x000000, "%s", s.c_str());
	DxLib::DrawLine(size.x / 2 - strwidth / 2, 70 + strheight, size.x / 2 + strwidth / 2, 70 + strheight, 0xff0000, 10);

	// 問題の手
	DxLib::DrawExtendGraph(size.x / 2 - 150, 200, size.x / 2 + 150, 500, handImg[0], true);
	DxLib::DrawLine(size.x / 2 - 150, 200, size.x / 2 - 150, 500, 0xff0000, 10);
	DxLib::DrawLine(size.x / 2 + 150, 200, size.x / 2 + 150, 500, 0xff0000, 10);
	DxLib::DrawLine(size.x / 2 + 150, 500, size.x / 2 - 150, 500, 0xff0000, 10);
	DxLib::DrawLine(size.x / 2 + 150, 200, size.x / 2 - 150, 200, 0xff0000, 10);

	DxLib::DrawString(20, 20, "<例題>", 0x000000);

	DxLib::SetFontSize(80);
	DxLib::DrawString(20, 220, "相手の手を見て、\n問題文にあう答えの\nボタンをクリック！", 0x000000);

	auto rect = buttons[2]->GetButtonRect();
	DxLib::DrawString(rect.center.x - rect.Width() / 2, rect.center.y - rect.Height(), "この問題では\nパーが正解\n　↓↓↓", 0x000000);

	if ((count / 40) % 2 == 0)
	{
		int strWidth, strHeight;
		GetDrawStringSize(&strWidth, &strHeight, nullptr, "左クリックでスタート!", strlen("左クリックでスタート!"));
		DrawString(size.x / 2 - strWidth / 2, size.y - strHeight - 50, "左クリックでスタート!", 0x0000ff);
	}
}

void Game1::GameDraw()
{
	// グーちょきぱーボタンの描画
	for (unsigned int i = 0; i < buttons.size(); ++i)
	{
		buttons[i]->Draw();

		auto rect = buttons[i]->GetButtonRect();
		DxLib::DrawExtendGraph(rect.center.x - rect.Width() / 3, rect.center.y - rect.Height() / 3,
						rect.center.x + rect.Width() / 3, rect.center.y + rect.Height() / 3, handImg[i], true);
	}

	if (updater != &Game1::WaitUpdate)
	{
		DxLib::SetFontSize(96);
		DxLib::DrawFormatString(50, 50, 0x000000, "第%d問", nowQNum);
		// 問題文章の描画
		std::string s = questionStatements[qStatementNum];
		int strwidth, strheight;
		GetDrawStringSize(&strwidth, &strheight, nullptr, s.c_str(), strlen(s.c_str()));
		auto size = Game::Instance().GetScreenSize();
		DxLib::DrawFormatString(size.x / 2 - strwidth / 2, 70, 0x000000, "%s", s.c_str());

		// 問題の手
		DxLib::DrawExtendGraph(size.x / 2 - 150, 200, size.x / 2 + 150, 500, handImg[qHandNum], true);
	}
	else
	{
		DxLib::SetFontSize(96*2);
		auto size = Game::Instance().GetScreenSize();
		int strWidth, strHeight;
		if (waitTime <= 60)
		{
			GetDrawStringSize(&strWidth, &strHeight, nullptr, "スタート!", strlen("スタート!"));
			DrawString(size.x / 2 - strWidth / 2, size.y / 10 - strHeight / 2, "スタート！", 0x000000);
		}
		else
		{
			GetDrawStringSize(&strWidth, &strHeight, nullptr, "0", strlen("0"));
			DrawFormatString(size.x / 2 - strWidth / 2, size.y / 10 - strHeight / 2, 0x000000, "%d", (waitTime / 60));
		}

	}

	if (updater == &Game1::AnswerDisplayUpdate)
	{
		if (result)
		{
			auto size = Game::Instance().GetScreenSize();
			Vector2 imgSize;
			GetGraphSize(_correctImg, &imgSize.x, &imgSize.y);
			DrawGraph(size.x / 2 - imgSize.x / 2, size.y / 2 - imgSize.y / 2, _correctImg, true);
		}
		else
		{
			auto size = Game::Instance().GetScreenSize();
			Vector2 imgSize;
			GetGraphSize(_missImg, &imgSize.x, &imgSize.y);
			DrawRotaGraph(size.x - imgSize.x / 2, size.y - imgSize.y / 2, 0.6, 0, _missImg, true);
		}
	}
}

void Game1::CreateQuestion()
{
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());

	qStatementNum = engine() % questionStatements.size();
	qHandNum = engine() % questionHands.size();
}

Result Game1::JudgeResult(int & qNum, Hand & myHand)
{
	Result r;

	if (qNum == static_cast<int>(Hand::ROCK))
	{
		if (myHand == Hand::ROCK)
		{
			r = Result::DRAW;
		}
		else if (myHand == Hand::SCISSORS)
		{
			r = Result::LOSE;
		}
		else if (myHand == Hand::PAPER)
		{
			r = Result::WIN;
		}
	}
	else if (qNum == static_cast<int>(Hand::SCISSORS))
	{
		if (myHand == Hand::ROCK)
		{
			r = Result::WIN;
		}
		else if (myHand == Hand::SCISSORS)
		{
			r = Result::DRAW;
		}
		else if (myHand == Hand::PAPER)
		{
			r = Result::LOSE;
		}
	}
	else if (qNum == static_cast<int>(Hand::PAPER))
	{
		if (myHand == Hand::ROCK)
		{
			r = Result::LOSE;
		}
		else if (myHand == Hand::SCISSORS)
		{
			r = Result::WIN;
		}
		else if (myHand == Hand::PAPER)
		{
			r = Result::DRAW;
		}
	}
	return r;
}

Game1::Game1()
{
	updater = &Game1::FadeinUpdate;
	drawer = &Game1::TitleDraw;

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/rock.png", data);
	handImg.emplace_back(data.GetHandle());
	Game::Instance().GetFileSystem()->Load("img/scissors.png", data);
	handImg.emplace_back(data.GetHandle());
	Game::Instance().GetFileSystem()->Load("img/paper.png", data);
	handImg.emplace_back(data.GetHandle());

	Game::Instance().GetFileSystem()->Load("img/Button/red.png", data);
	rock = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/Button/yellow.png", data);
	scissors = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/Button/blue.png", data);
	paper = data.GetHandle();

	Game::Instance().GetFileSystem()->Load("img/Game2/maru.png", data);
	_correctImg = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/Game2/batu.png", data);
	_missImg = data.GetHandle();

	SoundData sData;
	Game::Instance().GetFileSystem()->Load("SE/correct1.mp3", sData);
	trueSE = sData.GetHandle();
	Game::Instance().GetFileSystem()->Load("SE/incorrect1.mp3", sData);
	falseSE = sData.GetHandle();
	Game::Instance().GetFileSystem()->Load("SE/countDown.mp3", sData);
	_cntDownSE = sData.GetHandle();
	Game::Instance().GetFileSystem()->Load("SE/start.mp3", sData);
	_startSE = sData.GetHandle();
	Game::Instance().GetFileSystem()->Load("BGM/game.mp3", sData);
	_gameBGM = sData.GetHandle();
	
	buttons.emplace_back(new Button(Rect(305, 830, 400, 400), rock));
	buttons.emplace_back(new Button(Rect(955, 830, 400, 400), scissors));
	buttons.emplace_back(new Button(Rect(1605, 830, 400, 400), paper));

	questionHands[static_cast<int>(Hand::ROCK)] = rock;
	questionHands[static_cast<int>(Hand::SCISSORS)] = scissors;
	questionHands[static_cast<int>(Hand::PAPER)] = paper;
	qHandNum = 0;

	questionStatements[0] = "勝ってください";
	questionStatements[1] = "負けてください";
	questionStatements[2] = "勝たないでください";
	questionStatements[3] = "負けないでください";
	qStatementNum = 0;

	waitTime = 240;
	count = 0;

	displayCount = 30;
	nowQNum = 1;

	result = false;
	trueNum = 0;

	CreateQuestion();
}

Game1::~Game1()
{
	DeleteSoundMem(trueSE);
	DeleteSoundMem(falseSE);
	DeleteSoundMem(_cntDownSE);
	DeleteSoundMem(_startSE);
	DeleteSoundMem(_gameBGM);
}

void Game1::Update(const Peripheral & p)
{
	(this->*updater)(p);
}

void Game1::Draw()
{
	auto size = Game::Instance().GetScreenSize();
	DxLib::DrawBox(0, 0, size.x, size.y, 0xffffff, true);
	(this->*drawer)();
}
