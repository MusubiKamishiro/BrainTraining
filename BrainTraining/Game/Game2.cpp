#include "Game2.h"

#include <DxLib.h>
#include <string>

#include "../Peripheral.h"
#include "../Game.h"

#include "../Scene/SceneManager.h"
#include "../Scene/ResultScene.h"
#include "../Scene/PauseScene.h"

constexpr int MAX_QUESTIONS = 20;	// 問題数

void Game2::NumInit(Num &num)
{
	num.money_1 = 0;
	num.money_5 = 0;
	num.money_10 = 0;
	num.money_50 = 0;
	num.money_100 = 0;
	num.money_500 = 0;
	num.money_1000 = 0;
	num.total = 0;
}

void Game2::Total(Num &num)
{
	num.total =
		num.money_1 * 1 +
		num.money_5 * 5 +
		num.money_10 * 10 +
		num.money_50 * 50 +
		num.money_100 * 100 +
		num.money_500 * 500 +
		num.money_1000 * 1000;
}

void Game2::Question()
{
	do
	{
		NumInit(_right);
		NumInit(_left);
		switch (_questions)
		{
		case 1:
		case 6:
		case 11:
		case 16:
			_right.money_1 = GetRand(9) + 1;
			_right.type = DrawType::Money;
			_left.money_1 = GetRand(9) + 1;
			_left.type = DrawType::Money;
			break;
		case 2:
		case 7:
		case 12:
		case 17:
			_right.money_10 = GetRand(7) + 2;
			_right.type = DrawType::Money;
			_left.money_50 = 1;
			_left.type = DrawType::Money;
			break;
		case 3:
		case 8:
		case 13:
		case 18:
			_right.money_5 = GetRand(5) + 10;
			_right.type = DrawType::Number;
			_left.money_5 = GetRand(5) + 10;
			_left.type = DrawType::Money;
			break;
		case 4:
		case 9:
		case 14:
		case 19:
			_right.money_1000 = 1;
			_right.type = DrawType::Number;
			_left.money_100 = GetRand(5) + 7;
			_left.type = DrawType::Money;
			break;
		case 5:
		case 10:
		case 15:
		case 20:
			_right.money_500 = GetRand(3) + 1;
			_right.type = DrawType::Money;
			_left.money_100 = GetRand(10) + 5;
			_left.type = DrawType::Money;
			break;
		default:
			_right.money_100 = GetRand(10);
			_right.type = DrawType::Number;
			_left.money_100 = GetRand(10);
			_left.type = DrawType::Number;
			break;
		}

		Total(_right);
		Total(_left);
	} while (_right.total == _left.total);
}

void Game2::Correct()
{
	ChangeVolumeSoundMem(200, _SE_correct);
	PlaySoundMem(_SE_correct, DX_PLAYTYPE_BACK);
	++_correctNum;

	_updater = &Game2::AnswerUpdate;
	_drawer = &Game2::CorrectDraw;
}

void Game2::Incorrect()
{
	ChangeVolumeSoundMem(200, _SE_miss);
	PlaySoundMem(_SE_miss, DX_PLAYTYPE_BACK);

	_updater = &Game2::AnswerUpdate;
	_drawer = &Game2::IncorrectDraw;
}

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
		_count = 200;
		_updater = &Game2::CountDownUpdate;
		_drawer = &Game2::CountDownDraw;
	}
}

void Game2::CountDownUpdate(const Peripheral & p)
{
	if (!(_count % 60) && _count >= 0)
	{
		if (_count == 0)
		{
			PlaySoundMem(_SE_start, DX_PLAYTYPE_BACK);
		}
		else
		{
			PlaySoundMem(_SE_count, DX_PLAYTYPE_BACK);
		}

	}
	--_count;
	if (_count < 0)
	{
		if (!CheckSoundMem(_SE_start))
		{
			Question();

			_updater = &Game2::WaitUpdate;
			_drawer = &Game2::WaitDraw;
		}
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
			// 左をクリック
			if (pos.x < size.x / 2)
			{
				if (_left.total > _right.total)
				{
					Correct();
				}
				else
				{
					Incorrect();
				}
			}
			// 右をクリック
			else
			{
				if (_right.total > _left.total)
				{
					Correct();
				}
				else
				{
					Incorrect();
				}
			}
		}
	}

	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}
}

void Game2::AnswerUpdate(const Peripheral & p)
{
	if (!CheckSoundMem(_SE_correct) && !CheckSoundMem(_SE_miss))
	{
		++_questions;
		if (_questions > MAX_QUESTIONS)
		{
			_updater = &Game2::FadeoutUpdate;
		}
		else
		{
			Question();

			_updater = &Game2::WaitUpdate;
			_drawer = &Game2::WaitDraw;
		}
	}
}

void Game2::StartDraw()
{
	auto size = Game::Instance().GetScreenSize();

	DxLib::DrawBox(0, 0, size.x, size.y, 0xffffff, true);

	DrawRotaGraph(size.x / 2, size.y / 2, 1.5, 0.0, _img_balance, true);

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(250);
	GetDrawStringSize(&strwidth, &strheight, nullptr, "数字比べ", strlen("数字比べ"));
	DrawString(size.x / 2 - strwidth / 2, size.y / 3 - strheight / 2, "数字比べ", 0x2222ff);

	SetFontSize(100);
	GetDrawFormatStringSize(&strwidth, &strheight, nullptr, "全%d問", MAX_QUESTIONS);
	DxLib::DrawFormatString(size.x / 2 - strwidth / 2, size.y / 3 * 2 - strheight / 2, 0x000000, "全%d問", MAX_QUESTIONS);

	GetDrawStringSize(&strwidth, &strheight, nullptr, "値の大きいほうをクリックしよう！", strlen("値の大きいほうをクリックしよう！"));
	DrawString(size.x / 2 - strwidth / 2, size.y / 3 * 2 - strheight / 2 + 150, "値の大きいほうをクリックしよう！", 0x000000);
}

void Game2::CountDownDraw()
{
	auto size = Game::Instance().GetScreenSize();
	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(150);
	if (_count < 0)
	{
		GetDrawStringSize(&strwidth, &strheight, nullptr, "スタート!", strlen("スタート!"));
		DrawString(size.x / 2 - strwidth / 2, size.y / 2 - strheight / 2, "スタート！", 0x000000);
	}
	else
	{
		GetDrawStringSize(&strwidth, &strheight, nullptr, "0", strlen("0"));
		DrawFormatString(size.x / 2 - strwidth / 2, size.y / 2 - strheight / 2, 0x000000, "%d", min((_count / 60) + 1, 3));
	}
}

void Game2::WaitDraw()
{
	DrawQuestion();
}

void Game2::IncorrectDraw()
{
	auto size = Game::Instance().GetScreenSize();

	DrawQuestion();

	DrawRotaGraph(
		_left.total < _right.total ? size.x / 4 : size.x / 4 * 3,
		size.y / 3 * 2,
		0.5, 0.0, _img_batu, true);
}

void Game2::CorrectDraw()
{
	auto size = Game::Instance().GetScreenSize();

	DrawQuestion();

	DrawRotaGraph(
		_left.total > _right.total ? size.x / 4 : size.x / 4 * 3,
		size.y / 3 * 2,
		1.0, 0.0, _img_maru, true);
}

void Game2::DrawQuestion()
{
	auto size = Game::Instance().GetScreenSize();

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(400);

	switch (_right.type)
	{
	case DrawType::Number:
		GetDrawFormatStringSize(&strwidth, &strheight, nullptr, "%d", _right.total);
		DxLib::DrawFormatString(size.x / 4 * 3 - strwidth / 2 + 4, size.y / 3 * 2 - strheight / 2 + 4, 0x000000, "%d", _right.total);
		DxLib::DrawFormatString(size.x / 4 * 3 - strwidth / 2, size.y / 3 * 2 - strheight / 2, 0xffffff, "%d", _right.total);
		break;
	case DrawType::Money:
		DrawMoney(_right, size.x / 2);
		break;
	}

	switch (_left.type)
	{
	case DrawType::Number:
		GetDrawFormatStringSize(&strwidth, &strheight, nullptr, "%d", _left.total);
		DxLib::DrawFormatString(size.x / 4 - strwidth / 2 + 4, size.y / 3 * 2 - strheight / 2 + 4, 0x000000, "%d", _left.total);
		DxLib::DrawFormatString(size.x / 4 - strwidth / 2, size.y / 3 * 2 - strheight / 2, 0xffffff, "%d", _left.total);
		break;
	case DrawType::Money:
		DrawMoney(_left);
		break;
	}
}

void Game2::DrawMoney(Num num, int offset)
{
	auto size = Game::Instance().GetScreenSize();

	int idx = 0;
	float mag = 0.5f;

	for (int i = 0; i < num.money_1000; ++i)
	{
		DrawGraph(offset + 100 + i * 20, size.y / 3 + i * 20, _img_1000en, true);
	}
	for (int i = 0; i < num.money_500; ++i)
	{
		auto pos = _positions[idx];
		DrawRotaGraph(offset + pos.x, pos.y, mag, 0.0, _img_500en, true);
		++idx;
	}
	for (int i = 0; i < num.money_100; ++i)
	{
		auto pos = _positions[idx];
		DrawRotaGraph(offset + pos.x, pos.y, mag, 0.0, _img_100en, true);
		++idx;
	}
	for (int i = 0; i < num.money_50; ++i)
	{
		auto pos = _positions[idx];
		DrawRotaGraph(offset + pos.x, pos.y, mag, 0.0, _img_50en, true);
		++idx;
	}
	for (int i = 0; i < num.money_10; ++i)
	{
		auto pos = _positions[idx];
		DrawRotaGraph(offset + pos.x, pos.y, mag, 0.0, _img_10en, true);
		++idx;
	}
	for (int i = 0; i < num.money_5; ++i)
	{
		auto pos = _positions[idx];
		DrawRotaGraph(offset + pos.x, pos.y, mag, 0.0, _img_5en, true);
		++idx;
	}
	for (int i = 0; i < num.money_1; ++i)
	{
		auto pos = _positions[idx];
		DrawRotaGraph(offset + pos.x, pos.y, mag, 0.0, _img_1en, true);
		++idx;
	}
}

Game2::Game2()
{
	NumInit(_right);
	_right.type = DrawType::Number;
	NumInit(_left);
	_left.type = DrawType::Number;

	_questions = 1;
	_correctNum = 0;

	_SE_start = LoadSoundMem("SE/start.mp3");
	_SE_count = LoadSoundMem("SE/countDown.mp3");
	_SE_correct = LoadSoundMem("SE/correct1.mp3");
	_SE_miss = LoadSoundMem("SE/incorrect.mp3");

	_img_balance = LoadGraph("img/Game2/balance.png");
	_img_maru = LoadGraph("img/Game2/maru.png");
	_img_batu = LoadGraph("img/Game2/batu.png");

	_img_1en = LoadGraph("img/Game2/一円.png");
	_img_5en = LoadGraph("img/Game2/五円.png");
	_img_10en = LoadGraph("img/Game2/十円.png");
	_img_50en = LoadGraph("img/Game2/五十円.png");
	_img_100en = LoadGraph("img/Game2/百円.png");
	_img_500en = LoadGraph("img/Game2/五百円.png");
	_img_1000en = LoadGraph("img/Game2/千円.png");

	auto size = Game::Instance().GetScreenSize();
	float angle = 0;
	_positions.resize(20);
	for (int i = 0;i < _positions.size(); ++i)
	{
		_positions[i] = Vector2(size.x / 4, size.y / 3 * 2 - 70);
		_positions[i].x += sin(DX_PI_F / 180.0f * angle) * (80 + 120 * (angle / 360));
		_positions[i].y += cos(DX_PI_F / 180.0f * angle) * (80 + 120 * (angle / 360));
		angle += 90 / (1 + angle / 360);
	}

	_updater = &Game2::FadeinUpdate;
	_drawer = &Game2::StartDraw;
}


Game2::~Game2()
{
	DeleteSoundMem(_SE_correct);
	DeleteSoundMem(_SE_miss);

	DeleteGraph(_img_1en);
	DeleteGraph(_img_5en);
	DeleteGraph(_img_10en);
	DeleteGraph(_img_50en);
	DeleteGraph(_img_100en);
	DeleteGraph(_img_1000en);
}

void Game2::Update(const Peripheral & p)
{
	(this->*_updater)(p);
}

void Game2::Draw()
{
	auto size = Game::Instance().GetScreenSize();

	DxLib::DrawBox(0, 0, size.x, size.y, 0xffffff, true);
	DxLib::DrawBox(0, size.y / 4, size.x / 2, size.y, 0xffcccc, true);
	DxLib::DrawBox(size.x / 2, size.y / 4, size.x, size.y, 0xccccff, true);

	DrawLine(0, size.y / 4, size.x, size.y / 4, 0x000000, 5);
	DrawLine(size.x / 2, size.y / 4, size.x / 2, size.y, 0x000000, 5);

	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawRotaGraph(size.x / 2 + 15, size.y / 2, 1.5, 0.0, _img_balance, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(100);
	GetDrawStringSize(&strwidth, &strheight, nullptr, "数字の大きい方をクリック！", strlen("数字の大きい方をクリック！"));
	DxLib::DrawFormatString(size.x / 2 - strwidth / 2, size.y / 8 - strheight / 2, 0x000000, "数字の大きい方をクリック！");

	(this->*_drawer)();
}
