#include "Game3.h"

#include <DxLib.h>
#include <time.h>

#include "../Peripheral.h"

#include "../Scene/SceneManager.h"
#include "../Scene/ResultScene.h"
#include "../Scene/PauseScene.h"



void Game3::FadeinUpdate(const Peripheral & p)
{
	if (pal > 255)
	{
		pal = 255;
		updater = &Game3::WaitUpdate;
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
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		pal = 255;
		updater = &Game3::FadeoutUpdate;
	}
	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}

	static int debug = 0;
	static int cnt = 0;

	auto GetRandom = [](const int& min, const int& max, const int& lastNum)
	{
		int num = min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));

		while (num == lastNum)
		{
			num = min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
		}
		return num;
	};

	if (cnt % 60 == 0)
	{
		_lastNum = GetRandom(0, _texts.size() - 1, _lastNum);
		auto flag = ChangeJudgeFlag(_lastNum);
		while (!flag)
		{
			_lastNum = GetRandom(0, _texts.size() - 1, _lastNum);
			flag	 = ChangeJudgeFlag(_lastNum);
		}
	
	}
	cnt++;

	/// debug用の描画
	if (_judgeFlag.first)
	{
		DrawCircle(600, 120, 50, 0xff0000, true);
	}
	else
	{
		DrawCircle(600, 200, 50, 0xff0000, true);
	}

	if (_judgeFlag.second)
	{
		DrawCircle(900, 120, 50, 0xffffff, true);
	}
	else
	{
		DrawCircle(900, 200, 50, 0xffffff, true);
	}
	DrawFormatString(100, 100, 0xffffff, "乱数の値 : %d", _lastNum);
	DrawExtendString(300, 700, 3.0, 3.0, _texts[_lastNum].c_str(), 0xffffff);

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
			return true;
		}
		else if (num % 2 == 1 && !_judgeFlag.second)
		{
			return true;
		}
	}
	else if (num / 2 == static_cast<int>(JFLAG::STAYDOWN))
	{

		if (num % 2 == 0 && _judgeFlag.first)
		{
			return true;
		}
		else if (num % 2 == 1 && _judgeFlag.second)
		{
			return true;
		}
	}
	return false;
}

Game3::Game3()
{
	_texts.emplace_back("赤あげて！");
	_texts.emplace_back("白あげて！");
	_texts.emplace_back("赤さげて！");
	_texts.emplace_back("白さげて！");
	_texts.emplace_back("赤あげないて！");
	_texts.emplace_back("白あげないて！");
	_texts.emplace_back("赤さげないて！");
	_texts.emplace_back("白さげないて！");

	_judgeFlag = { false, false };

	updater = &Game3::FadeinUpdate;
}


Game3::~Game3()
{
}

void Game3::Update(const Peripheral & p)
{
	
	(this->*updater)(p);
}

void Game3::Draw()
{
	DxLib::DrawBox(0, 0, 100, 100, 0x0000ff, true);
	DxLib::DrawString(450, 450, "ゲームシーン[3]だよ", 0xffffff);
}
