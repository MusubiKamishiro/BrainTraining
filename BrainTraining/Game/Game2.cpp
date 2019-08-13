#include "Game2.h"

#include <DxLib.h>
#include <string>

#include "../Peripheral.h"
#include "../Game.h"

#include "../Scene/SceneManager.h"
#include "../Scene/ResultScene.h"
#include "../Scene/PauseScene.h"

constexpr int MAX_QUESTIONS = 10;	// 問題数

void Game2::FadeinUpdate(const Peripheral & p)
{
	if (pal > 255)
	{
		pal = 255;
		_updater = &Game2::StartUpdate;
		_drawer = &Game2::StartDraw;
	}
	else
	{
		pal += 20;
	}
}

void Game2::FadeoutUpdate(const Peripheral & p)
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

void Game2::StartUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		ChangeVolumeSoundMem(125, _SE_question);
		PlaySoundMem(_SE_question, DX_PLAYTYPE_BACK);

		_updater = &Game2::QuestionsUpdate;
		_drawer = &Game2::QuestionsDraw;
	}
}

void Game2::WaitUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		auto size = Game::Instance().GetScreenSize();
		auto pos = p.GetMousePos();

		// 左をクリック
		if (pos.x < size.x / 2)
		{
			if (_left > _right)
			{
				ChangeVolumeSoundMem(200, _SE_correct);
				PlaySoundMem(_SE_correct, DX_PLAYTYPE_BACK);
			}
			else
			{
				ChangeVolumeSoundMem(200, _SE_miss);
				PlaySoundMem(_SE_miss, DX_PLAYTYPE_BACK);
			}
		}
		// 右をクリック
		else
		{
			if (_right > _left)
			{
				ChangeVolumeSoundMem(200, _SE_correct);
				PlaySoundMem(_SE_correct, DX_PLAYTYPE_BACK);
			}
			else
			{
				ChangeVolumeSoundMem(200, _SE_miss);
				PlaySoundMem(_SE_miss, DX_PLAYTYPE_BACK);
			}
		}

		_updater = &Game2::AnswerUpdate;
		_drawer = &Game2::AnswerDraw;
	}

	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}
}

void Game2::QuestionsUpdate(const Peripheral & p)
{
	if (!CheckSoundMem(_SE_question))
	{
		do
		{
			_right = GetRand(100);
			_left = GetRand(100);
		} 
		while (_right == _left);

		_updater = &Game2::WaitUpdate;
		_drawer = &Game2::WaitDraw;
	}
}

void Game2::AnswerUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		++_questions;
		if (_questions > MAX_QUESTIONS)
		{
			_updater = &Game2::FadeoutUpdate;
		}
		else
		{
			ChangeVolumeSoundMem(125, _SE_question);
			PlaySoundMem(_SE_question, DX_PLAYTYPE_BACK);

			_updater = &Game2::QuestionsUpdate;
			_drawer = &Game2::QuestionsDraw;
		}
	}
}

void Game2::StartDraw()
{
	auto size = Game::Instance().GetScreenSize();

	DxLib::DrawBox(0, 0, size.x, size.y, 0xffffff, true);

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(250);
	GetDrawStringSize(&strwidth, &strheight, nullptr, "数字比べ", strlen("数字比べ"));
	DrawString(size.x / 2 - strwidth / 2, size.y / 2 - strheight / 2, "数字比べ", 0x000000);
}

void Game2::WaitDraw()
{
}

void Game2::QuestionsDraw()
{
	auto size = Game::Instance().GetScreenSize();

	DxLib::DrawBox(0, 0, size.x, size.y, 0xffffff, true);

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(250);
	GetDrawFormatStringSize(&strwidth, &strheight, nullptr, "第%d問", _questions);
	DrawFormatString(size.x / 2 - strwidth / 2, size.y / 2 - strheight / 2, 0x000000, "第%d問", _questions);
}

void Game2::AnswerDraw()
{
	auto size = Game::Instance().GetScreenSize();

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(500);

	GetDrawStringSize(&strwidth, &strheight, nullptr, "×", strlen("×"));
	DrawString(
		_left < _right ? size.x / 4 - strwidth / 2 : size.x / 4 * 3 - strwidth / 2,
		size.y / 3 * 2 - strheight / 2,
		"×", 0x000000);

	GetDrawStringSize(&strwidth, &strheight, nullptr, "〇", strlen("〇"));
	DrawString(
		_left > _right ? size.x / 4 - strwidth / 2 : size.x / 4 * 3 - strwidth / 2,
		size.y / 3 * 2 - strheight / 2,
		"〇", 0x000000);
}

Game2::Game2()
{
	_right = 0;
	_left = 0;
	_questions = 1;

	_SE_question = LoadSoundMem("SE/question1.mp3");
	_SE_correct = LoadSoundMem("SE/correct1.mp3");
	_SE_miss = LoadSoundMem("SE/incorrect1.mp3");

	_updater = &Game2::FadeinUpdate;
	_drawer = &Game2::StartDraw;
}


Game2::~Game2()
{
	DeleteSoundMem(_SE_question);
	DeleteSoundMem(_SE_correct);
	DeleteSoundMem(_SE_miss);
}

void Game2::Update(const Peripheral & p)
{
	(this->*_updater)(p);
}

void Game2::Draw()
{
	auto size = Game::Instance().GetScreenSize();

	DxLib::DrawBox(0, 0, size.x / 2, size.y, 0xff0000, true);
	DxLib::DrawBox(size.x / 2, 0, size.x, size.y, 0x0000ff, true);
	DxLib::DrawBox(0, 0, size.x, size.y / 4, 0xffffff, true);

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(24);
	strwidth = GetDrawStringWidth("ゲームシーン[2]だよ", strlen("ゲームシーン[2]だよ"));
	DrawFormatString(size.x / 2 - strwidth / 2, 50, 0x0000000, "ゲームシーン[2]だよ");

	SetFontSize(50);
	strwidth = GetDrawStringWidth("数字の大きい方をクリック！", strlen("数字の大きい方をクリック！"));
	DrawFormatString(size.x / 2 - strwidth / 2, 150, 0x000000, "数字の大きい方をクリック！");

	SetFontSize(500);

	GetDrawFormatStringSize(&strwidth, &strheight, nullptr, "%d", _right);
	DrawFormatString(size.x / 4 * 3 - strwidth / 2 + 4, size.y / 3 * 2 - strheight / 2 + 4, 0x000000, "%d", _right);
	DrawFormatString(size.x / 4 * 3 - strwidth / 2, size.y / 3 * 2 - strheight / 2, 0xffffff, "%d", _right);

	GetDrawFormatStringSize(&strwidth, &strheight, nullptr, "%d", _left);
	DrawFormatString(size.x / 4 - strwidth / 2 + 4, size.y / 3 * 2 - strheight / 2 + 4, 0x000000, "%d", _left);
	DrawFormatString(size.x / 4 - strwidth / 2, size.y / 3 * 2 - strheight / 2, 0xffffff, "%d", _left);

	(this->*_drawer)();
}
