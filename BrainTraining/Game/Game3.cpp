#include "Game3.h"

#include <DxLib.h>

#include "../Game.h"
#include "../Peripheral.h"
#include "../Button.h"

#include "../System/FileSystem.h"
#include "../System/ImageLoader.h"

#include "../Scene/SceneManager.h"
#include "../Scene/ResultScene.h"
#include "../Scene/PauseScene.h"

void Game3::FadeinUpdate(const Peripheral & p)
{
	if (pal > 255)
	{
		pal = 255;
		_updater = &Game3::StartUpdate;
	}
	else
	{
		pal += 20;
	}
}

void Game3::FadeoutUpdate(const Peripheral & p)
{
	if (pal <= 0)
	{
		SceneManager::Instance().ChangeScene(std::make_unique<ResultScene>(_questions, _corrects));
	}
	else
	{
		pal -= 20;
	}
}

void Game3::WaitUpdate(const Peripheral & p)
{
	if (!CheckSoundMem(_correctSE) || !CheckSoundMem(_missSE))
	{
		if (_questions >= 20)
		{
			_updater = &Game3::FadeoutUpdate;
		}
		else
		{
			if (_questions >= 2)
			{
				_updater = &Game3::GameUpdate;
			}
			else
			{
				_updater = &Game3::FirstUpdate;
			}
		}
	}
}

void Game3::StartUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		_updater = &Game3::FirstUpdate;
		_drawer = &Game3::GameDraw;
	}
}

void Game3::FirstUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}

	if (_judgeFlag.first && _judgeFlag.second)
	{
		_updater = &Game3::GameUpdate;
	}

	if (!_isJudge)
	{
		_isJudge = true;
		if (!_judgeFlag.first)
		{
			_lastNum = 0;
			_judgeFlag.first = true;
		}
		else
		{
			_lastNum = 1;
			_judgeFlag.second = true;
		}
	}
	else
	{
		ButtonUpdater(p);
	}
}

void Game3::GameUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}

	// 乱数を出すためのﾗﾑﾀﾞ式
	auto GetRandom = [](const int& min, const int& max, const int& lastNum)
	{
		int num = min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));

		/// 前回の値を同じ場合、もう一度乱数を出す
		while (num == lastNum)
		{
			num = min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
		}
		return num;
	};

	if (!_isJudge)
	{
		_isJudge = true;
		_lastNum = GetRandom(0, _texts.size() - 1, _lastNum);
		auto flag = ChangeJudgeFlag(_lastNum);
		while (!flag)
		{
			/// 前と同じ乱数の結果が出たとき、もう一度乱数を求めている
			_lastNum = GetRandom(0, _texts.size() - 1, _lastNum);
			flag = ChangeJudgeFlag(_lastNum);
		}
	}
	else
	{
		ButtonUpdater(p);
	}
}

bool Game3::ChangeJudgeFlag(const int & num)
{
	if (num / 2 == static_cast<int>(JFLAG::UP))
	{
		if (num % 2 == 0 && !_judgeFlag.first)
		{
			_judgeFlag.first = true;
			return true;
		}
		else if (num % 2 == 1 && !_judgeFlag.second)
		{
			_judgeFlag.second = true;
			return true;
		}
		else {}
	}
	else if (num / 2 == static_cast<int>(JFLAG::DOWN))
	{
		if (num % 2 == 0 && _judgeFlag.first)
		{
			_judgeFlag.first = false;
			return true;
		}
		else if (num % 2 == 1 && _judgeFlag.second)
		{
			_judgeFlag.second = false;
			return true;
		}
		else {}
	}
	else if (num / 2 == static_cast<int>(JFLAG::STAYUP))
	{
		if (num % 2 == 0 && !_judgeFlag.first)
		{
			_judgeFlag.first = true;
		}
		else if (num % 2 == 1 && !_judgeFlag.second)
		{
			_judgeFlag.second = true;
		}
		else {}
		return true;
	}
	else if (num / 2 == static_cast<int>(JFLAG::STAYDOWN))
	{
		if (num % 2 == 0 && _judgeFlag.first)
		{
			_judgeFlag.first = false;
		}
		else if (num % 2 == 1 && _judgeFlag.second)
		{
			_judgeFlag.second = false;
		}
		else {}
		return true;
	}
	else {}
	return false;
}

void Game3::ChangeFlag(const BUTTON & btn)
{
	if (btn == BUTTON::RED)
	{
		_plFlag.first = !_plFlag.first;
	}
	else if (btn == BUTTON::WHITE)
	{
		_plFlag.second = !_plFlag.second;
	}
	else {}

}

void Game3::ButtonUpdater(const Peripheral& p)
{
	auto btn = _buttons.begin();
	for (; btn != _buttons.end(); ++btn)
	{
		auto cnt = btn - _buttons.begin();
		if ((*btn)->Update(p))
		{
			ChangeFlag((BUTTON)cnt);
			_isJudge = false;
			/// 旗の判定
			if (_judgeFlag == _plFlag)
			{
				++_corrects;
				++_questions;
				PlaySoundMem(_correctSE, DX_PLAYTYPE_BACK);
			}
			else
			{
				_plFlag = _judgeFlag;
				++_questions;
				PlaySoundMem(_missSE, DX_PLAYTYPE_BACK);
			}
			_updater = &Game3::WaitUpdate;
		}
	}
}

Game3::Game3() : _btnSize(Size(300, 150))
{
	/// 指示用のテキストを追加している
	_texts.emplace_back("赤あげて！");
	_texts.emplace_back("白あげて！");
	_texts.emplace_back("赤さげて！");
	_texts.emplace_back("白さげて！");
	_texts.emplace_back("赤さげないて！");
	_texts.emplace_back("白さげないて！");
	_texts.emplace_back("赤あげないて！");
	_texts.emplace_back("白あげないて！");

	_judgeFlag = _plFlag = { false, false };

	/// ﾎﾞﾀﾝの設定
	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/up.png", data);
	_upImg = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/down.png", data);
	_downImg = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/stay.png", data);
	_stayImg = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/flag.png", data);
	_flagImg = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/flag2.png", data);
	_flag2Img = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/flag3.png", data);
	_flag3Img = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/flag4.png", data);
	_flag4Img = data.GetHandle();

	auto size = Game::Instance().GetScreenSize();
	_buttons.emplace_back(new Button(Rect(size.x / 7 + _btnSize.width / 2, size.y / 4 * 3 + _btnSize.height / 2,
										  _btnSize.width, _btnSize.height)));
	_buttons.emplace_back(new Button(Rect(size.x / 5 * 2 + _btnSize.width / 2, size.y / 4 * 3 + _btnSize.height / 2, 
										  _btnSize.width, _btnSize.height)));
	_buttons.emplace_back(new Button(Rect(size.x / 3 * 2 + _btnSize.width / 2, size.y / 4 * 3 + _btnSize.height / 2,
										  _btnSize.width, _btnSize.height)));

	_correctSE = LoadSoundMem("SE/correct1.mp3");
	_missSE	= LoadSoundMem("SE/incorrect1.mp3");

	_isJudge = false;
	_questions = _corrects = 0;

	_updater = &Game3::FadeinUpdate;
	_drawer = &Game3::StartDraw;
}

Game3::~Game3()
{
}

void Game3::Update(const Peripheral & p)
{
	(this->*_updater)(p);
}

void Game3::Draw()
{
	(this->*_drawer)();
}

void Game3::StartDraw()
{
	auto size = Game::Instance().GetScreenSize();

	DxLib::DrawBox(0, 0, size.x, size.y, 0xffffff, true);

	int strWidth, strHeight;
	strWidth = strHeight = 0;

	SetFontSize(250);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "旗揚げゲーム", strlen("旗揚げゲーム"));
	DrawString(size.x / 2 - strWidth / 2, size.y / 2 - strHeight / 2, "旗揚げゲーム", 0x000000);

}

void Game3::GameDraw()
{
	auto size = Game::Instance().GetScreenSize();

	DxLib::DrawBox(0, 0, size.x, size.y, 0xdddddd, true);

	int strWidth, strHeight;
	strWidth = strHeight = 0;

	SetFontSize(120);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, _texts[_lastNum].c_str(), strlen(_texts[_lastNum].c_str()));
	DrawString((size.x / 2 - strWidth / 2), (size.y / 13 * 9 - strHeight / 2), _texts[_lastNum].c_str(), 0x000000);

	if (_plFlag.first && !_plFlag.second)
	{
		DrawGraph((size.x / 4), 0, _flagImg, true);
	}
	else if (!_plFlag.first && _plFlag.second)
	{
		DrawGraph((size.x / 4), 0, _flag2Img, true);
	}
	else if (_plFlag.first && _plFlag.second)
	{
		DrawGraph((size.x / 4), 0, _flag3Img, true);
	}
	else
	{
		DrawGraph((size.x / 4), 0, _flag4Img, true);
	}

	for (auto btn : _buttons)
	{
		btn->Draw();
	}
	DxLib::DrawString((size.x / 6), (size.y / 13 * 9 - strHeight / 2), "赤", 0xff0000);
	/// 赤い旗のボタン
	if (!_plFlag.first)
	{
		DxLib::DrawExtendGraph((size.x / 7), (size.y / 4 * 3), (size.x / 7 + _btnSize.width), (size.y / 4 * 3 + _btnSize.height), _upImg, true);
	}
	else
	{
		DxLib::DrawExtendGraph((size.x / 7), (size.y / 4 * 3), (size.x / 7 + _btnSize.width), (size.y / 4 * 3 + _btnSize.height), _downImg, true);
	}
	DxLib::DrawString(size.x / 4 * 3, size.y / 13 * 9 - strHeight / 2, "白", 0xffffff);
	/// 白い旗のボタン
	if (!_plFlag.second)
	{
		DxLib::DrawExtendGraph((size.x / 3 * 2), (size.y / 4 * 3), (size.x / 3 * 2 + _btnSize.width), (size.y / 4 * 3 + _btnSize.height), _upImg, true);
	}
	else
	{
		DxLib::DrawExtendGraph((size.x / 3 * 2), (size.y / 4 * 3), (size.x / 3 * 2 + _btnSize.width), (size.y / 4 * 3 + _btnSize.height), _downImg, true);
	}
	/// そのままボタン
	DxLib::DrawExtendGraph((size.x / 5 * 2), (size.y / 4 * 3), (size.x / 5 * 2 + _btnSize.width), (size.y / 4 * 3 + _btnSize.height), _stayImg, true);
}