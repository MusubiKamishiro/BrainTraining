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
	if (!CheckSoundMem(_correctSE) && !CheckSoundMem(_missSE))
	{
		if (_questions >= 20)
		{
			_updater = &Game3::FadeoutUpdate;
		}
		else
		{
			if (_questions >= 2)
			{
				_moveFlagCnt = (_questions >= 10 ? 2 : 1);
				_updater = &Game3::GameUpdate;
			}
			else
			{
				_updater = &Game3::FirstUpdate;
			}
			_orderText = "";
			_timeCnt = ((_questions / 5 % 2) == 1 ? _defTime - (_defTime / 3) - 1 : _defTime - 1);	
			_isJudge = false;
		}
	}
}

void Game3::StartUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		_updater = &Game3::ExpUpdate;
		_drawer  = &Game3::ExpDraw;
	}
}

void Game3::FirstUpdate(const Peripheral & p)
{
	if (!CheckSoundMem(_gameBGM))
	{
		PlaySoundMem(_gameBGM, DX_PLAYTYPE_BACK);
	}

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
			_orderText += "赤" + _texts[1] + "!";
			_judgeFlag.first = true;
		}
		else
		{
			_orderText += "白" + _texts[1] + "!";
			_judgeFlag.second = true;
		}
	}
	else
	{
		ButtonUpdater(p);
	}

	if (_timeCnt < 0)
	{
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

	--_timeCnt;
}

void Game3::GameUpdate(const Peripheral & p)
{
	if (!CheckSoundMem(_gameBGM))
	{
		PlaySoundMem(_gameBGM, DX_PLAYTYPE_BACK);
	}
	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}

	if (!_isJudge)
	{
		JudgeFlagUpdater();
	}
	else
	{
		/// 制限時間内ならボタンを押し放題にする
		ButtonUpdater(p);
	}

	if (_timeCnt < 0)
	{
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
	--_timeCnt;
}

void Game3::ExpUpdate(const Peripheral & p)
{
	++_blindCnt;
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		++_expCnt;
		if (_expCnt >= _expImgs.size())
		{
			_blindCnt = 0;
			_timeCnt = 240;
			PlaySoundMem(_cntDownSE, DX_PLAYTYPE_BACK);
			_updater = &Game3::CntDownUpdate;
			_drawer  = &Game3::CntDownDraw;
		}
	}
}

void Game3::CntDownUpdate(const Peripheral & p)
{
	--_timeCnt;
	if (!(_timeCnt % 60) && _timeCnt > 0)
	{
		if (_timeCnt / 60 == 1)
		{
			PlaySoundMem(_startSE, DX_PLAYTYPE_BACK);
		}
		else
		{
			PlaySoundMem(_cntDownSE, DX_PLAYTYPE_BACK);
		}
	}
	if (_timeCnt <= 0 && !CheckSoundMem(_startSE))
	{
		_timeCnt = _defTime - 1;
		_updater = &Game3::FirstUpdate;
		_drawer  = &Game3::GameDraw;
	}
}

void Game3::MoveJudgeFlag(const int& num, const FLAG& btn)
{
	if (num / 2 == static_cast<int>(ORDER::UP))
	{
		if (btn == FLAG::RED)
		{
			_judgeFlag.first = true;
		}
		else if (btn == FLAG::WHITE)
		{
			_judgeFlag.second = true;
		}
		else {}
	}
	else if (num / 2 == static_cast<int>(ORDER::DOWN))
	{
		if (btn == FLAG::RED)
		{
			_judgeFlag.first = false;
		}
		else if (btn == FLAG::WHITE)
		{
			_judgeFlag.second = false;
		}
		else {}
	}
	else {}
}

void Game3::MovePlFlag(const FLAG & btn)
{
	if (btn == FLAG::RED)
	{
		_plFlag.first = !_plFlag.first;
	}
	else if (btn == FLAG::WHITE)
	{
		_plFlag.second = !_plFlag.second;
	}
	else {}
}

void Game3::JudgeFlagUpdater()
{
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

	if (_moveFlagCnt <= 1)
	{
		_lastNum = GetRandom(0, _texts.size() - 1, _lastNum);
		auto btnNum = rand() % 2;
		MoveJudgeFlag(_lastNum, (FLAG)(btnNum));
		_orderText += (btnNum == 0 ? "赤" : "白");
		_orderText += _texts[_lastNum] + "!";
	}
	else
	{
		for (int i = 0; i < _moveFlagCnt; ++i)
		{
			/// 前回出した値と同じ値も出すようにしている
			_lastNum = GetRandom(0, _texts.size() - 1, 5);
			MoveJudgeFlag(_lastNum, (FLAG)(i));
			_orderText += (i == 0 ? "赤" : "白");
			if (i == 0)
			{
				_orderText += (_judgeFlag.first ? _texts[0] + "、" : _texts[2] + "、");
			}
			else
			{
				_orderText += (_judgeFlag.second ? _texts[1] + "!" : _texts[3] + "!");
			}
		}
	}
	_isJudge = true;
}

void Game3::ButtonUpdater(const Peripheral& p)
{
	auto btn = _buttons.begin();
	for (; btn != _buttons.end(); ++btn)
	{
		auto cnt = btn - _buttons.begin();
		if ((*btn)->Update(p))
		{
			MovePlFlag((FLAG)cnt);
		}
	}
}

Game3::Game3() : _defTime(180)
{
	/// 指示用のテキストを追加している
	_texts.emplace_back("さげないで");
	_texts.emplace_back("あげて");
	_texts.emplace_back("あげないで");
	_texts.emplace_back("さげて");

	_judgeFlag = _plFlag = { false, false };

	ImageData data;
	/// 旗を上げるｷｬﾗｸﾀｰの画像ﾊﾝﾄﾞﾙ読み込み
	Game::Instance().GetFileSystem()->Load("img/flag.png", data);
	_flagImgs.emplace_back(data.GetHandle());
	Game::Instance().GetFileSystem()->Load("img/flag2.png", data);
	_flagImgs.emplace_back(data.GetHandle());
	Game::Instance().GetFileSystem()->Load("img/flag3.png", data);
	_flagImgs.emplace_back(data.GetHandle());
	Game::Instance().GetFileSystem()->Load("img/flag4.png", data);
	_flagImgs.emplace_back(data.GetHandle());

	/// 説明用の画像ﾊﾝﾄﾞﾙ読み込み
	Game::Instance().GetFileSystem()->Load("img/game3/説明.png", data);
	_expImgs.emplace_back(data.GetHandle());
	Game::Instance().GetFileSystem()->Load("img/game3/説明2.png", data);
	_expImgs.emplace_back(data.GetHandle());
	Game::Instance().GetFileSystem()->Load("img/game3/説明3.png", data);
	_expImgs.emplace_back(data.GetHandle());

	/// ﾀｲﾏｰの画像ﾊﾝﾄﾞﾙ読み込み
	Game::Instance().GetFileSystem()->Load("img/timer.png", data);
	_timerImg = data.GetHandle();

	Game::Instance().GetFileSystem()->Load("img/Game2/maru.png", data);
	_correctImg = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/Game2/batu.png", data);
	_missImg = data.GetHandle();

	auto size = Game::Instance().GetScreenSize();
	_buttons.emplace_back(new Button(Rect(size.x / 4,size.y / 2,size.x / 2, size.y)));
	_buttons.emplace_back(new Button(Rect(size.x / 4 * 3, size.y / 2, size.x / 2, size.y)));

	_correctSE  = LoadSoundMem("SE/correct1.mp3");
	_missSE		= LoadSoundMem("SE/incorrect1.mp3");
	_cntDownSE  = LoadSoundMem("SE/countDown.mp3");
	_startSE    = LoadSoundMem("SE/start.mp3");
	_gameBGM	= LoadSoundMem("BGM/game.mp3");

	_orderText  = "";
	_timeCnt	= _defTime;

	_isJudge   = false;
	_questions = _corrects = _moveFlagCnt = _expCnt = 0;

	_updater = &Game3::FadeinUpdate;
	_drawer  = &Game3::StartDraw;
}

Game3::~Game3()
{
	DeleteSoundMem(_correctSE);
	DeleteSoundMem(_missSE);
	DeleteSoundMem(_cntDownSE);
	DeleteSoundMem(_startSE);
	DeleteSoundMem(_gameBGM);
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

	/// ゲーム開始前の描画
	DxLib::DrawBox(0, 0, size.x, size.y, 0xffffff, true);

	int strWidth, strHeight;
	strWidth = strHeight = 0;

	SetFontSize(200);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "旗上げゲーム", strlen("旗上げゲーム"));
	DrawString(size.x / 2 - strWidth / 2, size.y / 7 * 3 - strHeight, "旗上げゲーム", 0x000000);

	SetFontSize(100);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "全20問", strlen("全20問"));
	DrawString(size.x / 2 - strWidth / 2, size.y / 2 - strHeight / 2, "全20問", 0x000000);

	SetFontSize(120);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "出されたお題に合わせて", strlen("出されたお題に合わせて"));
	DrawString(size.x / 2 - strWidth / 2, size.y / 3 * 2 - strHeight / 2, "出されたお題に合わせて", 0xcc0000);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "旗を上げ下げするゲームだよ!", strlen("旗を上げ下げするゲームだよ!"));
	DrawString(size.x / 2 - strWidth / 2, size.y / 5 * 4 - strHeight / 2, "旗を上げ下げするゲームだよ!", 0xcc0000);
}

void Game3::ExpDraw()
{
	auto size = Game::Instance().GetScreenSize();

	/// 説明用の描画
	DrawGraph(0, 0, _expImgs[_expCnt], true);

	if ((_blindCnt / 25) % 2)
	{
		int strWidth, strHeight;
		SetFontSize(120);
		if (_expCnt == _expImgs.size() - 1)
		{
			GetDrawStringSize(&strWidth, &strHeight, nullptr, "左クリックでスタート!", strlen("左クリックでスタート!"));
			DrawString(size.x / 2 - strWidth / 2, size.y - strHeight, "左クリックでスタート!", 0x0000ff);
		}
		else
		{
			GetDrawStringSize(&strWidth, &strHeight, nullptr, "左クリックで次へ進む", strlen("左クリックで次へ進む"));
			DrawString(size.x / 2 - strWidth / 2, size.y - strHeight, "左クリックで次へ進む", 0x0000ff);
		}
	}
}

void Game3::CntDownDraw()
{
	auto size = Game::Instance().GetScreenSize();
	int strWidth, strHeight;
	Vector2 imgSize;
	GetGraphSize(_flagImgs[3], &imgSize.x, &imgSize.y);

	/// カウントダウンの描画
	DrawBox(0, 0, size.x / 2, size.y, 0xffcccc, true);
	DrawBox(size.x / 2, 0, size.x, size.y, 0xffffff, true);

	DrawGraph((size.x / 2 - imgSize.x / 2), size.y / 5, _flagImgs[3], true);

	SetFontSize(150);
	if (_timeCnt <= 60)
	{
		GetDrawStringSize(&strWidth, &strHeight, nullptr, "スタート!", strlen("スタート!"));
		DrawString(size.x / 2 - strWidth / 2, size.y / 10 - strHeight / 2, "スタート！", 0x000000);
	}
	else
	{
		GetDrawStringSize(&strWidth, &strHeight, nullptr, "0", strlen("0"));
		DrawFormatString(size.x / 2 - strWidth / 2, size.y / 10 - strHeight / 2, 0x000000, "%d", (_timeCnt / 60));
	}	
}

void Game3::GameDraw()
{
	/// ゲーム中の描画
	auto size = Game::Instance().GetScreenSize();
	DxLib::DrawBox(0, 0, size.x, size.y, 0xe0ffe0, true);

	DrawBox(0, 0, size.x / 2, size.y, 0xffcccc, true);
	DrawBox(size.x / 2, 0, size.x, size.y, 0xffffff, true);

	int strWidth, strHeight;
	strWidth = strHeight = 0;
	SetFontSize(130);
	
	/// 指示テキストの描画
	GetDrawStringSize(&strWidth, &strHeight, nullptr, _orderText.c_str(), strlen(_orderText.c_str()));
	DrawString((size.x / 2 - strWidth / 2), strHeight / 3, _orderText.c_str(), 0x000000);

	/// 制限時間の描画
	auto time = (_timeCnt <= 0 ? 0 : (_timeCnt / 60) + 1);
	auto color = (time <= 1 ? 0xff0000 : 0x000000);
	Vector2 imgSize;
	GetGraphSize(_timerImg, &imgSize.x, &imgSize.y);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "0", strlen("0"));
	DrawFormatString(size.x - strWidth / 2 - imgSize.x / 2, strHeight / 2, color, "%d", time);

	GetDrawStringSize(&strWidth, &strHeight, nullptr, "左クリックで旗を上げる(下げる)よ!", strlen("左クリックで旗を上げる(下げる)よ!"));
	DrawString(size.x / 2 - strWidth / 2, size.y - strHeight, "左クリックで旗を上げる(下げる)よ!", 0x0000aa);
	
	/// タイマーの描画
	DrawGraph(size.x - imgSize.x, 0, _timerImg, true);
	
	/// 旗を上げるキャラクターの描画
	GetGraphSize(_flagImgs[0], &imgSize.x, &imgSize.y);
	if (_plFlag.first && !_plFlag.second)
	{
		DrawGraph((size.x / 2 - imgSize.x / 2), size.y / 5, _flagImgs[0], true);
	}
	else if (!_plFlag.first && _plFlag.second)
	{
		DrawGraph((size.x / 2 - imgSize.x / 2), size.y / 5, _flagImgs[1], true);
	}
	else if (_plFlag.first && _plFlag.second)
	{
		DrawGraph((size.x / 2 - imgSize.x / 2), size.y / 5, _flagImgs[2], true);
	}
	else
	{
		DrawGraph((size.x / 2 - imgSize.x / 2), size.y / 5, _flagImgs[3], true);
	}

	if (CheckSoundMem(_correctSE))
	{
		GetGraphSize(_correctImg, &imgSize.x, &imgSize.y);
		DrawGraph(size.x / 2 - imgSize.x / 2, size.y / 2 - imgSize.y / 2, _correctImg, true);
	}

	if (CheckSoundMem(_missSE))
	{
		GetGraphSize(_missImg, &imgSize.x, &imgSize.y);
		DrawRotaGraph(size.x - imgSize.x / 2, size.y - imgSize.y / 2, 0.6, 0, _missImg, true);
	}
}