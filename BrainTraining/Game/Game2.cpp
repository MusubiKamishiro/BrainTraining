#include "Game2.h"

#include <DxLib.h>
#include <string>

#include "../Peripheral.h"
#include "../Game.h"

#include "../Scene/SceneManager.h"
#include "../Scene/ResultScene.h"
#include "../Scene/PauseScene.h"

constexpr int MAX_QUESTIONS = 10;	// ñ‚ëËêî

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
		SceneManager::Instance().ChangeScene(std::make_unique<ResultScene>(MAX_QUESTIONS, _correctNum));
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

		if (pos.y > size.y / 4)
		{
			// ç∂ÇÉNÉäÉbÉN
			if (pos.x < size.x / 2)
			{
				if (_left.num > _right.num)
				{
					ChangeVolumeSoundMem(200, _SE_correct);
					PlaySoundMem(_SE_correct, DX_PLAYTYPE_BACK);
					++_correctNum;
				}
				else
				{
					ChangeVolumeSoundMem(200, _SE_miss);
					PlaySoundMem(_SE_miss, DX_PLAYTYPE_BACK);
				}
			}
			// âEÇÉNÉäÉbÉN
			else
			{
				if (_right.num > _left.num)
				{
					ChangeVolumeSoundMem(200, _SE_correct);
					PlaySoundMem(_SE_correct, DX_PLAYTYPE_BACK);
					++_correctNum;
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
			switch (_questions)
			{
			case 1:
			case 6:
				_right.num = GetRand(9);
				_right.type = DrawType::Number;
				_left.num = GetRand(9);
				_left.type = DrawType::Number;
				break;
			case 2:
			case 7:
				_right.num = GetRand(999);
				_right.type = DrawType::Money;
				_left.num = GetRand(999);
				_left.type = DrawType::Money;
				break;
			case 3:
			case 8:
				_right.num = GetRand(999);
				_right.type = DrawType::Number;
				_left.num = GetRand(999);
				_left.type = DrawType::Money;
				break;
			case 4:
			case 9:
				_right.num = GetRand(50) + 500;
				_right.type = DrawType::Money;
				_left.num = GetRand(50) + 500;
				_left.type = DrawType::Number;
				break;
			case 5:
			case 10:
				_right.num = GetRand(100) + 3000;
				_right.type = DrawType::Money;
				_left.num = GetRand(100) + 3000;
				_left.type = DrawType::Money;
				break;
			default:
				_right.num = GetRand(10);
				_right.type = DrawType::Number;
				_left.num = GetRand(10);
				_left.type = DrawType::Number;
				break;
			}
		} 
		while (_right.num == _left.num);

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
	GetDrawStringSize(&strwidth, &strheight, nullptr, "êîéöî‰Ç◊", strlen("êîéöî‰Ç◊"));
	DrawString(size.x / 2 - strwidth / 2, size.y / 3 - strheight / 2, "êîéöî‰Ç◊", 0x000000);

	SetFontSize(100);
	GetDrawFormatStringSize(&strwidth, &strheight, nullptr, "ëS%dñ‚", MAX_QUESTIONS);
	DrawFormatString(size.x / 2 - strwidth / 2, size.y / 3 * 2 - strheight / 2, 0x000000, "ëS%dñ‚", MAX_QUESTIONS);
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
	GetDrawFormatStringSize(&strwidth, &strheight, nullptr, "ëÊ%dñ‚", _questions);
	DrawFormatString(size.x / 2 - strwidth / 2, size.y / 2 - strheight / 2, 0x000000, "ëÊ%dñ‚", _questions);
}

void Game2::AnswerDraw()
{
	auto size = Game::Instance().GetScreenSize();

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(500);

	GetDrawStringSize(&strwidth, &strheight, nullptr, "Å~", strlen("Å~"));
	DrawString(
		_left.num < _right.num ? size.x / 4 - strwidth / 2 : size.x / 4 * 3 - strwidth / 2,
		size.y / 3 * 2 - strheight / 2,
		"Å~", 0x0000ff, 0xffffff);

	GetDrawStringSize(&strwidth, &strheight, nullptr, "ÅZ", strlen("ÅZ"));
	DrawString(
		_left.num > _right.num ? size.x / 4 - strwidth / 2 : size.x / 4 * 3 - strwidth / 2,
		size.y / 3 * 2 - strheight / 2,
		"ÅZ", 0xff0000, 0xffffff);
}

void Game2::DrawMoney(int num, int offset)
{
	auto size = Game::Instance().GetScreenSize();

	auto sen	= num / 1000;
	auto hyaku	= num % 1000 / 100;
	auto juu	= num % 100 / 10;
	auto iti	= num % 10;

	for (int i = 0; i < sen; ++i)
	{
		DrawGraph(offset + 100 + i * 20, size.y / 3 + i * 20, _img_1000en, true);
	}
	for (int i = 0; i < hyaku; ++i)
	{
		DrawRotaGraph(offset + 100 + i * 20, size.y / 3 * 2 - 100 + i * 20, 0.5, 0.0, _img_100en, true);
	}
	for (int i = 0; i < juu; ++i)
	{
		DrawRotaGraph(offset + 400 + i * 20, size.y / 3 * 2 - 100 + i * 20, 0.5, 0.0, _img_10en, true);
	}
	for (int i = 0; i < iti; ++i)
	{
		DrawRotaGraph(offset + 700 + i * 20, size.y / 3 * 2 - 100 + i * 20, 0.5, 0.0, _img_1en, true);
	}
}

Game2::Game2()
{
	_right.num = 0;
	_right.type = DrawType::Number;
	_left.num = 0;
	_left.type = DrawType::Money;

	_questions = 1;
	_correctNum = 0;

	_SE_question = LoadSoundMem("SE/question1.mp3");
	_SE_correct = LoadSoundMem("SE/correct1.mp3");
	_SE_miss = LoadSoundMem("SE/incorrect1.mp3");

	_img_1en = LoadGraph("img/àÍâ~.png");
	_img_5en = LoadGraph("img/å‹â~.png");
	_img_10en = LoadGraph("img/è\â~.png");
	_img_50en = LoadGraph("img/å‹è\â~.png");
	_img_100en = LoadGraph("img/ïSâ~.png");
	_img_1000en = LoadGraph("img/êÁâ~.png");
	_img_5000en = LoadGraph("img/å‹êÁâ~.png");
	_img_10000en = LoadGraph("img/àÍñúâ~.png");

	_updater = &Game2::FadeinUpdate;
	_drawer = &Game2::StartDraw;
}


Game2::~Game2()
{
	DeleteSoundMem(_SE_question);
	DeleteSoundMem(_SE_correct);
	DeleteSoundMem(_SE_miss);

	DeleteGraph(_img_1en);
	DeleteGraph(_img_5en);
	DeleteGraph(_img_10en);
	DeleteGraph(_img_50en);
	DeleteGraph(_img_100en);
	DeleteGraph(_img_1000en);
	DeleteGraph(_img_5000en);
	DeleteGraph(_img_10000en);
}

void Game2::Update(const Peripheral & p)
{
	(this->*_updater)(p);
}

void Game2::Draw()
{
	auto size = Game::Instance().GetScreenSize();

	DxLib::DrawBox(0, 0, size.x, size.y, 0xffffff, true);
	DxLib::DrawBox(0, size.y / 4, size.x / 2, size.y, 0xff0000, true);
	DxLib::DrawBox(size.x / 2, size.y / 4, size.x, size.y, 0x0000ff, true);
	DrawLine(0, size.y / 4, size.x, size.y / 4, 0x000000, 5);
	DrawLine(size.x / 2, size.y / 4, size.x / 2, size.y, 0x000000, 5);

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(100);
	GetDrawStringSize(&strwidth, &strheight, nullptr, "êîéöÇÃëÂÇ´Ç¢ï˚ÇÉNÉäÉbÉNÅI", strlen("êîéöÇÃëÂÇ´Ç¢ï˚ÇÉNÉäÉbÉNÅI"));
	DrawFormatString(size.x / 2 - strwidth / 2, size.y / 8 - strheight / 2, 0x000000, "êîéöÇÃëÂÇ´Ç¢ï˚ÇÉNÉäÉbÉNÅI");

	SetFontSize(400);

	switch (_right.type)
	{
	case DrawType::Number:
		GetDrawFormatStringSize(&strwidth, &strheight, nullptr, "%d", _right.num);
		DrawFormatString(size.x / 4 * 3 - strwidth / 2 + 4, size.y / 3 * 2 - strheight / 2 + 4, 0x000000, "%d", _right.num);
		DrawFormatString(size.x / 4 * 3 - strwidth / 2, size.y / 3 * 2 - strheight / 2, 0xffffff, "%d", _right.num);
		break;
	case DrawType::Money:
		DrawMoney(_right.num, size.x / 2);
		break;
	}

	switch (_left.type)
	{
	case DrawType::Number:
		GetDrawFormatStringSize(&strwidth, &strheight, nullptr, "%d", _left.num);
		DrawFormatString(size.x / 4 - strwidth / 2 + 4, size.y / 3 * 2 - strheight / 2 + 4, 0x000000, "%d", _left.num);
		DrawFormatString(size.x / 4 - strwidth / 2, size.y / 3 * 2 - strheight / 2, 0xffffff, "%d", _left.num);
		break;
	case DrawType::Money:
		DrawMoney(_left.num);
		break;
	}

	(this->*_drawer)();
}
