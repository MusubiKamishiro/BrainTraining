#include "Game6.h"

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

void Game6::FadeinUpdate(const Peripheral & p)
{
	if (pal > 255)
	{
		pal = 255;
		_updater = &Game6::StartUpdate;
	}
	else
	{
		pal += 20;
	}
}

void Game6::FadeoutUpdate(const Peripheral & p)
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

void Game6::WaitUpdate(const Peripheral & p)
{
	if (!CheckSoundMem(_correctSE) && !CheckSoundMem(_missSE))
	{
		if (_questions >= 20)
		{
			_updater = &Game6::FadeoutUpdate;
		}
		else
		{
			if (_questions >= 6)
			{
				SetColor();
			}
			_updater = &Game6::GameUpdate;
		}
	}
}

void Game6::StartUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		_updater = &Game6::ExpUpdate;
		_drawer  = &Game6::ExpDraw;
	}
}

void Game6::ExpUpdate(const Peripheral & p)
{
	/// 操作説明
	++_blindCnt;
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		++_expCnt;
		if (_expCnt >= _expImgs.size())
		{
			_blindCnt = 0;
			_timeCnt = 240;
			PlaySoundMem(_cntDownSE, DX_PLAYTYPE_BACK);
			_updater = &Game6::CntDownUpdate;
			_drawer = &Game6::CntDownDraw;
		}
	}
}

void Game6::CntDownUpdate(const Peripheral & p)
{
	/// カウントダウン
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
		_updater = &Game6::GameUpdate;
		_drawer = &Game6::GameDraw;
	}
}

void Game6::GameUpdate(const Peripheral & p)
{
	if (!CheckSoundMem(_gameBGM))
	{
		PlaySoundMem(_gameBGM, DX_PLAYTYPE_BACK);
	}
	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}

	if (!_isColor)
	{
		ChangeColor();
	}
	else
	{
		ButtonUpdater(p);
	}
}

void Game6::SetColor()
{
	_colorType.clear();
	for (int i = 0; i < _colors.size(); ++i)
	{
		if (_colorType.size() == 0)
		{
			_colorType.emplace_back((COLOR)(rand() % 4));
		}
		else
		{
			auto cnt = _colorType.size();
			while (_colorType.size() <= cnt)
			{
				auto randNum = rand() % 4;
				auto flag = true;
				for (auto type : _colorType)
				{
					/// 登録されている色と違う色の場合、trueを返す
					flag = (randNum != static_cast<int>(type));

					if (!flag)
					{
						break;
					}
				}

				if (flag)
				{
					_colorType.emplace_back((COLOR)(randNum));
				}
			}
		}
	}
}

void Game6::ChangeColor()
{
	// 乱数を出すためのﾗﾑﾀﾞ式
	auto GetRandom = [](const int& min, const int& max)
	{
		int num = min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
		return num;
	};

	_textNum = GetRandom(0, _texts.size() - 1);
	_colorNum = GetRandom(0, _colors.size() - 1);
	while (_textNum == _colorNum)
	{
		_colorNum = GetRandom(0, _colors.size() - 1);
	}
	_isColor = true;
}

void Game6::ButtonUpdater(const Peripheral& p)
{
	auto btn = _buttons.begin();
	for (; btn != _buttons.end(); ++btn)
	{
		if ((*btn)->Update(p))
		{
			auto cnt = btn - _buttons.begin();
			auto colorID = static_cast<int>(_colorType[cnt]);
			_isColor = false;
			/// 色の判定
			if (_colorNum == colorID)
			{
				/// 正解
				++_corrects;
				++_questions;
				PlaySoundMem(_correctSE, DX_PLAYTYPE_BACK);
			}
			else
			{
				/// 不正解
				++_questions;
				PlaySoundMem(_missSE, DX_PLAYTYPE_BACK);
			}
			_updater = &Game6::WaitUpdate;
		}
	}
}

Game6::Game6() : _btnSize(Size(300,300))
{
	/// ﾃｷｽﾄの追加
	_texts.emplace_back("あか");
	_texts.emplace_back("あお");
	_texts.emplace_back("きいろ");
	_texts.emplace_back("みどり");

	/// 色の追加
	_colors.emplace_back(0xff4d4d);		/// 赤
	_colors.emplace_back(0x305bff);		/// 青
	_colors.emplace_back(0xffda1f);		/// 黄
	_colors.emplace_back(0x37ff37);		/// 緑

	/// 色の種別追加
	_colorType.emplace_back(COLOR::RED);
	_colorType.emplace_back(COLOR::BLUE);
	_colorType.emplace_back(COLOR::YELLOW);
	_colorType.emplace_back(COLOR::GREEN);

	auto size = Game::Instance().GetScreenSize();

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/game6/説明.png", data);
	_expImgs.emplace_back(data.GetHandle());
	Game::Instance().GetFileSystem()->Load("img/game6/説明2.png", data);
	_expImgs.emplace_back(data.GetHandle());
	Game::Instance().GetFileSystem()->Load("img/game6/説明3.png", data);
	_expImgs.emplace_back(data.GetHandle());

	Game::Instance().GetFileSystem()->Load("img/Button/light blue.png", data);
	_buttons.emplace_back(new Button(Rect(size.x / 6, size.y / 8 * 5,
										  _btnSize.width, _btnSize.height), data.GetHandle()));
	_buttons.emplace_back(new Button(Rect(size.x / 8 * 3, size.y / 8 * 5,
										  _btnSize.width, _btnSize.height), data.GetHandle()));
	_buttons.emplace_back(new Button(Rect(size.x / 7 * 4, size.y / 8 * 5,
										  _btnSize.width, _btnSize.height), data.GetHandle()));
	_buttons.emplace_back(new Button(Rect(size.x / 9 * 7, size.y / 8 * 5,
										  _btnSize.width, _btnSize.height), data.GetHandle()));

	_correctSE = LoadSoundMem("SE/correct1.mp3");
	_missSE	   = LoadSoundMem("SE/incorrect1.mp3");
	_cntDownSE = LoadSoundMem("SE/countDown.mp3");
	_startSE   = LoadSoundMem("SE/start.mp3");
	_gameBGM   = LoadSoundMem("BGM/game.mp3");

	_textNum = _colorNum = _questions = _corrects = 0;

	ChangeColor();

	_updater = &Game6::FadeinUpdate;
	_drawer  = &Game6::StartDraw;
}

Game6::~Game6()
{
}

void Game6::Update(const Peripheral & p)
{
	(this->*_updater)(p);
}

void Game6::Draw()
{
	(this->*_drawer)();
}

void Game6::StartDraw()
{
	auto size = Game::Instance().GetScreenSize();

	DxLib::DrawBox(0, 0, size.x, size.y, 0xffffff, true);

	int strWidth, strHeight;
	strWidth = strHeight = 0;

	SetFontSize(150);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "文字の色当てゲーム", strlen("文字の色当てゲーム"));
	DrawString(size.x / 2 - strWidth / 2, size.y / 7 * 3 - strHeight / 2, "文字の色当てゲーム", 0x000000);

	SetFontSize(120);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "書かれた文字の色を", strlen("書かれた文字の色を"));
	DrawString(size.x / 2 - strWidth / 2, size.y / 3 * 2 - strHeight / 2, "書かれた文字の色を", 0xcc0000);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "当てるゲームだよ!", strlen("当てるゲームだよ!"));
	DrawString(size.x / 2 - strWidth / 2, size.y / 5 * 4 - strHeight / 2, "当てるゲームだよ!", 0xcc0000);
}

void Game6::ExpDraw()
{
	auto size = Game::Instance().GetScreenSize();
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

void Game6::CntDownDraw()
{
	auto size = Game::Instance().GetScreenSize();
	int strWidth, strHeight;

	DxLib::DrawBox(0, 0, size.x, size.y, 0xffffff, true);

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

	for (auto btn : _buttons)
	{
		btn->Draw();
	}

	SetFontSize(120);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "あか", strlen("あか"));
	DrawString((size.x / 6) - strWidth / 2, (size.y / 8 * 5) - strHeight / 2, "あか", 0x000000);
	DrawString((size.x / 8 * 3) - strWidth / 2, (size.y / 8 * 5) - strHeight / 2, "あお", 0x000000);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "きいろ", strlen("きいろ"));
	DrawString((size.x / 7 * 4) - strWidth / 2, (size.y / 8 * 5) - strHeight / 2, "きいろ", 0x000000);
	DrawString((size.x / 9 * 7) - strWidth / 2, (size.y / 8 * 5) - strHeight / 2, "みどり", 0x000000);
}

void Game6::GameDraw()
{
	auto size = Game::Instance().GetScreenSize();

	DxLib::DrawBox(0, 0, size.x, size.y, 0xffffff, true);

	int strWidth, strHeight;
	strWidth = strHeight = 0;
	SetFontSize(300);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, _texts[_textNum].c_str(), strlen(_texts[_textNum].c_str()));
	DrawString((size.x / 2 - strWidth / 2), (size.y / 3 - strHeight / 2), _texts[_textNum].c_str(), _colors[_colorNum]);
	for (auto btn : _buttons)
	{
		btn->Draw();
	}

	
	SetFontSize(120);
	/// ﾎﾞﾀﾝの文字座標
	std::vector<Vector2> pos;
	pos.emplace_back(Vector2((size.x / 6), (size.y / 8 * 5)));
	pos.emplace_back(Vector2((size.x / 8 * 3), (size.y / 8 * 5)));
	pos.emplace_back(Vector2((size.x / 7 * 4), (size.y / 8 * 5)));
	pos.emplace_back(Vector2((size.x / 9 * 7), (size.y / 8 * 5)));

	auto color = _colorType.begin();
	for (; color != _colorType.end(); ++color)
	{
		auto cnt = color - _colorType.begin();
		switch ((*color))
		{
		case COLOR::RED :
			GetDrawStringSize(&strWidth, &strHeight, nullptr, "あか", strlen("あか"));
			DrawString(pos[cnt].x - strWidth / 2, pos[cnt].y - strHeight / 2, "あか", 0x000000);
			break;
			
		case COLOR::BLUE:
			GetDrawStringSize(&strWidth, &strHeight, nullptr, "あお", strlen("あお"));
			DrawString(pos[cnt].x - strWidth / 2, pos[cnt].y - strHeight / 2, "あお", 0x000000);
			break;
		case COLOR::YELLOW :
			GetDrawStringSize(&strWidth, &strHeight, nullptr, "きいろ", strlen("きいろ"));
			DrawString(pos[cnt].x - strWidth / 2, pos[cnt].y - strHeight / 2, "きいろ", 0x000000);
			break;
		case COLOR::GREEN :
			GetDrawStringSize(&strWidth, &strHeight, nullptr, "みどり", strlen("みどり"));
			DrawString(pos[cnt].x - strWidth / 2, pos[cnt].y - strHeight / 2, "みどり", 0x000000);
		default:
			break;
		}
	}
}