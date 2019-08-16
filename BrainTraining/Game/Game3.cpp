#include "Game3.h"

#include <DxLib.h>
#include <time.h>

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
		SceneManager::Instance().ChangeScene(std::make_unique<ResultScene>());
	}
	else
	{
		pal -= 20;
	}
}

void Game3::WaitUpdate(const Peripheral & p)
{
	if (_judgeFlag == _plFlag)
	{
		if (!CheckSoundMem(_correctSE))
		{
			if (_startCnt <= 0)
			{
				_updater = &Game3::GameUpdate;
			}
			else
			{
				--_startCnt;
				_updater = &Game3::StartUpdate;
			}
		}
	}
	else
	{
		if (!CheckSoundMem(_missSE))
		{
			_updater = &Game3::FadeoutUpdate;
		}
	}
}

void Game3::StartUpdate(const Peripheral & p)
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

	auto btn = _buttons.begin();
	for (; btn != _buttons.end(); ++btn)
	{
		auto cnt = btn - _buttons.begin();
		if ((*btn)->Update(p))
		{
			ChangeFlag((BUTTON)cnt);
			_isJudge = false;
			
			if (_judgeFlag != _plFlag)
			{
				PlaySoundMem(_missSE, DX_PLAYTYPE_BACK);
			}
			else
			{
				PlaySoundMem(_correctSE, DX_PLAYTYPE_BACK);
			}
			_updater = &Game3::WaitUpdate;
		}
	}
}

void Game3::GameUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}

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
			_lastNum = GetRandom(0, _texts.size() - 1, _lastNum);
			flag = ChangeJudgeFlag(_lastNum);
		}
	}

	auto btn = _buttons.begin();
	for (; btn != _buttons.end(); ++btn)
	{
		auto cnt = btn - _buttons.begin();
		if ((*btn)->Update(p))
		{
			ChangeFlag((BUTTON)cnt);
			_isJudge = false;
			if (_judgeFlag != _plFlag)
			{
				PlaySoundMem(_missSE, DX_PLAYTYPE_BACK);
			}
			else
			{
				PlaySoundMem(_correctSE, DX_PLAYTYPE_BACK);
			}
			_updater = &Game3::WaitUpdate;
		}
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
		else{}
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
		else{}
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
		else{}
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
	else{}
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

Game3::Game3() : _timeCnt(180)
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

	_buttons.emplace_back(new Button(Rect(455, 850, 300, 150)));
	_buttons.emplace_back(new Button(Rect(955, 850, 300, 150)));
	_buttons.emplace_back(new Button(Rect(1455, 850, 300, 150)));

	_correctSE = LoadSoundMem("SE/correct1.mp3");
	_missSE	   = LoadSoundMem("SE/incorrect1.mp3");

	_isJudge = false;
	_startCnt = 2;

	_updater = &Game3::FadeinUpdate;
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
	auto size = Game::Instance().GetScreenSize();

	DxLib::DrawBox(0, 0, size.x, size.y, 0xdddddd, true);
	DxLib::DrawBox(0, 0, 100, 100, 0x0000ff, true);

	if (_plFlag.first && !_plFlag.second)
	{
		DrawGraph(size.x / 4, 0, _flagImg, true);
	}
	else if (!_plFlag.first && _plFlag.second)
	{
		DrawGraph(size.x / 4, 0, _flag2Img, true);
	}
	else if (_plFlag.first && _plFlag.second)
	{
		DrawGraph(size.x / 4, 0, _flag3Img, true);
	}
	else
	{
		DrawGraph(size.x / 4, 0, _flag4Img, true);
	}
	DrawExtendString(size.x / 3, size.y / 5 * 3, 4.0, 4.0, _texts[_lastNum].c_str(), 0x000000);

	for (auto btn : _buttons)
	{
		btn->Draw();
	}
	DxLib::DrawExtendString(size.x / 6, size.y / 5 * 3, 3.0, 3.0, "赤", 0xff0000);
	/// 赤い旗のボタン
	if (!_plFlag.first)
	{
		DxLib::DrawExtendGraph(310, 775, 610, 925, _upImg, true);
	}
	else
	{
		DxLib::DrawExtendGraph(310, 775, 610, 925, _downImg, true);
	}
	DxLib::DrawExtendString(size.x / 7 * 5, size.y / 5 * 3, 3.0, 3.0, "白", 0xffffff);
	/// 白い旗のボタン
	if (!_plFlag.second)
	{
		DxLib::DrawExtendGraph(1310, 775, 1610, 925, _upImg, true);
	}
	else
	{
		DxLib::DrawExtendGraph(1310, 775, 1610, 925, _downImg, true);
	}

	/// そのままボタン
	DxLib::DrawExtendGraph(810, 775, 1110, 925, _stayImg, true);
}
