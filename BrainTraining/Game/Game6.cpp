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
	/// �������
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
	/// �J�E���g�_�E��
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

void Game6::ChangeColor()
{
	// �������o�����߂����ގ�
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
		auto cnt = btn - _buttons.begin();
		if ((*btn)->Update(p))
		{
			_isColor = false;
			/// �F�̔���
			if (_colorNum == cnt)
			{
				/// ����
				++_corrects;
				++_questions;
				PlaySoundMem(_correctSE, DX_PLAYTYPE_BACK);
			}
			else
			{
				/// �s����
				++_questions;
				PlaySoundMem(_missSE, DX_PLAYTYPE_BACK);
			}
			_updater = &Game6::WaitUpdate;
		}
	}
}

Game6::Game6() : _btnSize(Size(300,300))
{
	/// ÷�Ă̒ǉ�
	_texts.emplace_back("����");
	_texts.emplace_back("����");
	_texts.emplace_back("������");
	_texts.emplace_back("�݂ǂ�");

	/// �F�̒ǉ�
	_colors.emplace_back(0xff4d4d);		/// ��
	_colors.emplace_back(0x305bff);		/// ��
	_colors.emplace_back(0xffda1f);		/// ��
	_colors.emplace_back(0x37ff37);		/// ��

	auto size = Game::Instance().GetScreenSize();

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/game6/����.png", data);
	_expImgs.emplace_back(data.GetHandle());
	Game::Instance().GetFileSystem()->Load("img/game6/����2.png", data);
	_expImgs.emplace_back(data.GetHandle());
	Game::Instance().GetFileSystem()->Load("img/game6/����3.png", data);
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

	ChangeFont("�ق̂��A���e�B�[�N��", DX_CHARSET_DEFAULT);

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
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "�����̐F���ăQ�[��", strlen("�����̐F���ăQ�[��"));
	DrawString(size.x / 2 - strWidth / 2, size.y / 7 * 3 - strHeight / 2, "�����̐F���ăQ�[��", 0x000000);

	SetFontSize(120);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "�����ꂽ�����̐F��", strlen("�����ꂽ�����̐F��"));
	DrawString(size.x / 2 - strWidth / 2, size.y / 3 * 2 - strHeight / 2, "�����ꂽ�����̐F��", 0xcc0000);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "���Ă�Q�[������!", strlen("���Ă�Q�[������!"));
	DrawString(size.x / 2 - strWidth / 2, size.y / 5 * 4 - strHeight / 2, "���Ă�Q�[������!", 0xcc0000);

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
			GetDrawStringSize(&strWidth, &strHeight, nullptr, "���N���b�N�ŃX�^�[�g!", strlen("���N���b�N�ŃX�^�[�g!"));
			DrawString(size.x / 2 - strWidth / 2, size.y - strHeight, "���N���b�N�ŃX�^�[�g!", 0x0000ff);
		}
		else
		{
			GetDrawStringSize(&strWidth, &strHeight, nullptr, "���N���b�N�Ŏ��֐i��", strlen("���N���b�N�Ŏ��֐i��"));
			DrawString(size.x / 2 - strWidth / 2, size.y - strHeight, "���N���b�N�Ŏ��֐i��", 0x0000ff);
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
		GetDrawStringSize(&strWidth, &strHeight, nullptr, "�X�^�[�g!", strlen("�X�^�[�g!"));
		DrawString(size.x / 2 - strWidth / 2, size.y / 10 - strHeight / 2, "�X�^�[�g�I", 0x000000);
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
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "����", strlen("����"));
	DrawString((size.x / 6) - strWidth / 2, (size.y / 8 * 5) - strHeight / 2, "����", 0x000000);
	DrawString((size.x / 8 * 3) - strWidth / 2, (size.y / 8 * 5) - strHeight / 2, "����", 0x000000);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "������", strlen("������"));
	DrawString((size.x / 7 * 4) - strWidth / 2, (size.y / 8 * 5) - strHeight / 2, "������", 0x000000);
	DrawString((size.x / 9 * 7) - strWidth / 2, (size.y / 8 * 5) - strHeight / 2, "�݂ǂ�", 0x000000);
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
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "����", strlen("����"));
	DrawString((size.x / 6)		- strWidth / 2, (size.y / 8 * 5) - strHeight / 2, "����", 0x000000);
	DrawString((size.x / 8 * 3) - strWidth / 2, (size.y / 8 * 5) - strHeight / 2, "����", 0x000000);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, "������", strlen("������"));
	DrawString((size.x / 7 * 4) - strWidth / 2, (size.y / 8 * 5) - strHeight / 2, "������", 0x000000);
	DrawString((size.x / 9 * 7) - strWidth / 2, (size.y / 8 * 5) - strHeight / 2, "�݂ǂ�", 0x000000);
}