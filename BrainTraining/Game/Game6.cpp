#include <DxLib.h>
#include <time.h>

#include "../Game.h"
#include "Game6.h"

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

void Game6::StartUpdate(const Peripheral & p)
{
	/// ���͂��m�F���ꂽ�Ƃ��A�Q�[�������Ɉڍs����
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

		/// �����p�摜�̕\�����I���ƁA�J�E���g�_�E���ֈڍs����
		if (_expCnt >= _expImgs.size())
		{
			_blindCnt = 0;
			_timeCnt = 240;
			PlaySoundMem(_cntDownSE, DX_PLAYTYPE_BACK);
			_updater = &Game6::CntDownUpdate;
			_drawer  = &Game6::CntDownDraw;
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

	/// �J�E���g�_�E���I�����A�Q�[���Ɉڍs����
	if (_timeCnt <= 0 && !CheckSoundMem(_startSE))
	{
		_updater = &Game6::GameUpdate;
		_drawer  = &Game6::GameDraw;
	}
}

void Game6::GameUpdate(const Peripheral & p)
{
	/// �Q�[������BGM
	if (!CheckSoundMem(_gameBGM))
	{
		PlaySoundMem(_gameBGM, DX_PLAYTYPE_BACK);
	}

	/// �|�[�Y��ʂ��J��
	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}

	if (!_isColor)
	{
		/// �����̐F���ύX����Ă��Ȃ��Ƃ��ɓ��鏈��
		ChangeColor();
	}
	else
	{
		/// �{�^�����͂̍X�V�p
		ButtonUpdater(p);
	}
}

void Game6::WaitUpdate(const Peripheral & p)
{
	/// �����A�s�������̑҂�����
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
				ChangeButton();
			}
			_updater = &Game6::GameUpdate;
		}
	}
}

void Game6::ChangeButton()
{
	/// �{�^���̏�����
	bool flag = true;
	int randNum = 0;
	_btnType.clear();
	for (int i = 0; i < _colors.size(); ++i)
	{
		/// �ŏ��̃{�^���̓o�^
		if (_btnType.size() == 0)
		{
			_btnType.emplace_back((COLOR)(rand() % 4));
		}
		else
		{
			auto cnt = _btnType.size();
			/// �{�^���̒ǉ�������܂ŁA���[�v������
			while (_btnType.size() <= cnt)
			{
				randNum = rand() % 4;
				flag = true;
				for (auto type : _btnType)
				{
					/// �o�^����Ă���{�^�����̔���
					flag = (randNum != static_cast<int>(type));

					if (!flag)
					{
						break;
					}
				}

				/// �{�^���̓o�^
				if (flag)
				{
					_btnType.emplace_back((COLOR)(randNum));
				}
			}
		}
	}

}

void Game6::ChangeColor()
{
	/// �����ƐF�̐ݒ���s���Ă���
	_textNum  = rand() % (_texts.size() - 1);
	_colorNum = rand() % (_colors.size() - 1);

	/// �����ƐF�������ꍇ�A�F�̕ύX������
	while (_textNum == _colorNum)
	{
		_colorNum = rand() % (_colors.size() - 1);
	}
	_isColor = true;
}

void Game6::ButtonUpdater(const Peripheral& p)
{
	auto btn = _buttons.begin();
	for (; btn != _buttons.end(); ++btn)
	{
		/// �{�^���������ꂽ�Ƃ��ɓ���
		if ((*btn)->Update(p))
		{
			/// �����ꂽ�{�^���̈ʒu���擾����
			auto cnt = btn - _buttons.begin();

			/// �F�̎擾
			auto color = static_cast<int>(_btnType[cnt]);

			/// �F�̔���
			if (_colorNum == color)
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
			_isColor = false;
			_updater = &Game6::WaitUpdate;
		}
	}
}

Game6::Game6() : _btnSize(Size(300,300))
{
	/// ÷�Ă̒ǉ�
	_texts[0] = "����";
	_texts[1] = "����";
	_texts[2] = "������";
	_texts[3] = "�݂ǂ�";

	/// �F�̒ǉ�
	_colors[0] = 0xff4d4d;		/// ��
	_colors[1] = 0x305bff;		/// ��
	_colors[2] = 0xffda1f;		/// ��
	_colors[3] = 0x37ff37;		/// ��

	/// �F�̎�ʒǉ�
	_btnType.reserve(static_cast<int>(COLOR::MAX));
	_btnType.emplace_back(COLOR::RED);
	_btnType.emplace_back(COLOR::BLUE);
	_btnType.emplace_back(COLOR::YELLOW);
	_btnType.emplace_back(COLOR::GREEN);

	/// �����p�摜�̏�����
	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/game6/����.png", data);
	_expImgs[0] = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/game6/����2.png", data);
	_expImgs[1] = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/game6/����3.png", data);
	_expImgs[2] = data.GetHandle();

	/// �{�^���̏�����
	Game::Instance().GetFileSystem()->Load("img/Button/light blue.png", data);

	/// �{�^���̃T�C�Y�Ɣz�u�ʒu�̐ݒ�
	auto size = Game::Instance().GetScreenSize();
	_buttons[0] = std::make_shared<Button>(Rect(size.x / 6, size.y / 8 * 5,
											    _btnSize.width, _btnSize.height), data.GetHandle());
	_buttons[1] = std::make_shared<Button>(Rect(size.x / 8 * 3, size.y / 8 * 5,
												_btnSize.width, _btnSize.height), data.GetHandle());
	_buttons[2] = std::make_shared<Button>(Rect(size.x / 7 * 4, size.y / 8 * 5,
											    _btnSize.width, _btnSize.height), data.GetHandle());
	_buttons[3] = std::make_shared<Button>(Rect(size.x / 9 * 7, size.y / 8 * 5,
											    _btnSize.width, _btnSize.height), data.GetHandle());

	/// �����A�s�����摜�̏�����
	Game::Instance().GetFileSystem()->Load("img/Game2/maru.png", data);
	_correctImg = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/Game2/batu.png", data);
	_missImg = data.GetHandle();

	/// ���ʉ��̐ݒ�
	_correctSE = LoadSoundMem("SE/correct1.mp3");
	_missSE	   = LoadSoundMem("SE/incorrect1.mp3");
	_cntDownSE = LoadSoundMem("SE/countDown.mp3");
	_startSE   = LoadSoundMem("SE/start.mp3");
	_gameBGM   = LoadSoundMem("BGM/game.mp3");

	/// �Q�[�����Ɏg�p����J�E���g�̏�����
	_textNum = _colorNum = _questions = _corrects = _expCnt = 0;

	ChangeColor();

	_updater = &Game6::FadeinUpdate;
	_drawer  = &Game6::StartDraw;
}

Game6::~Game6()
{
	DeleteSoundMem(_correctSE);
	DeleteSoundMem(_missSE);
	DeleteSoundMem(_cntDownSE);
	DeleteSoundMem(_startSE);
	DeleteSoundMem(_gameBGM);
}

void Game6::Update(const Peripheral & p)
{
	(this->*_updater)(p);
}

void Game6::Draw()
{
	/// �w�i�̕`��
	DxLib::DrawBox(0, 0, Game::Instance().GetScreenSize().x, Game::Instance().GetScreenSize().y, 0xffffff, true);

	/// �Q�[���̏�Ԃ��Ƃɕ`���ύX���Ă���
	(this->*_drawer)();
}

void Game6::StartDraw()
{
	auto size = Game::Instance().GetScreenSize();

	/// �Q�[�����̕`��
	SetFontSize(150);
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, "�����̐F���ăQ�[��", strlen("�����̐F���ăQ�[��"));
	DrawString(size.x / 2 - _strSize.x / 2, size.y / 7 * 3 - _strSize.y, "�����̐F���ăQ�[��", 0x000000);

	/// ��萔�̕`��
	SetFontSize(100);
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, "�S20��", strlen("�S20��"));
	DrawString(size.x / 2 - _strSize.x / 2, size.y / 2 - _strSize.y / 2, "�S20��", 0x000000);

	/// �����p�e�L�X�g�̕`��
	SetFontSize(120);
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, "�����ꂽ�����̐F��", strlen("�����ꂽ�����̐F��"));
	DrawString(size.x / 2 - _strSize.x / 2, size.y / 3 * 2 - _strSize.y / 2, "�����ꂽ�����̐F��", 0xcc0000);
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, "���Ă�Q�[������!", strlen("���Ă�Q�[������!"));
	DrawString(size.x / 2 - _strSize.x / 2, size.y / 5 * 4 - _strSize.y / 2, "���Ă�Q�[������!", 0xcc0000);
}

void Game6::ExpDraw()
{
	/// �����p�摜�̕`��
	DrawGraph(0, 0, _expImgs[_expCnt], true);

	/// �����p�e�L�X�g�̕`��
	auto size = Game::Instance().GetScreenSize();
	if ((_blindCnt / 25) % 2)
	{
		SetFontSize(120);
		if (_expCnt == _expImgs.size() - 1)
		{
			GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, "���N���b�N�ŃX�^�[�g!", strlen("���N���b�N�ŃX�^�[�g!"));
			DrawString(size.x / 2 - _strSize.x / 2, size.y - _strSize.y, "���N���b�N�ŃX�^�[�g!", 0x0000ff);
		}
		else
		{
			GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, "���N���b�N�Ŏ��֐i��", strlen("���N���b�N�Ŏ��֐i��"));
			DrawString(size.x / 2 - _strSize.x / 2, size.y - _strSize.y, "���N���b�N�Ŏ��֐i��", 0x0000ff);
		}
	}
}

void Game6::CntDownDraw()
{
	/// �J�E���g�_�E�����̕`��
	auto size = Game::Instance().GetScreenSize();
	SetFontSize(150);
	if (_timeCnt <= 60)
	{
		/// �Q�[���J�n��ʒm����`��
		GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, "�X�^�[�g!", strlen("�X�^�[�g!"));
		DrawString(size.x / 2 - _strSize.x / 2, size.y / 10 - _strSize.y / 2, "�X�^�[�g�I", 0x000000);
	}
	else
	{
		/// �J�E���g�_�E���̕`��
		GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, "0", strlen("0"));
		DrawFormatString(size.x / 2 - _strSize.x / 2, size.y / 10 - _strSize.y / 2, 0x000000, "%d", (_timeCnt / 60));
	}

	/// �{�^���̕`��
	for (auto btn : _buttons)
	{
		btn->Draw();
	}

	/// �{�^���̕����ʒu
	Vector2 pos[4];
	pos[0] = Vector2((size.x / 6), (size.y / 8 * 5));
	pos[1] = Vector2((size.x / 8 * 3), (size.y / 8 * 5));
	pos[2] = Vector2((size.x / 7 * 4), (size.y / 8 * 5));
	pos[3] = Vector2((size.x / 9 * 7), (size.y / 8 * 5));

	/// �{�^���̕����̕`��
	SetFontSize(120);
	auto text = _texts.begin();
	for (; text != _texts.end(); ++text)
	{
		auto cnt = text - _texts.begin();
		GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, (*text).c_str(), strlen((*text).c_str()));
		DrawString(pos[cnt].x - _strSize.x / 2, pos[cnt].y - _strSize.y / 2, (*text).c_str(), 0x000000);
	}
}

void Game6::GameDraw()
{
	auto size = Game::Instance().GetScreenSize();

	/// �w�i�̕`��
	DxLib::DrawBox(0, 0, size.x, size.y, 0xffffff, true);

	/// �Q�[�����̂���̕`��
	SetFontSize(300);
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _texts[_textNum].c_str(), strlen(_texts[_textNum].c_str()));
	DrawString((size.x / 2 - _strSize.x / 2), (size.y / 3 - _strSize.y / 2), _texts[_textNum].c_str(), _colors[_colorNum]);
	for (auto btn : _buttons)
	{
		btn->Draw();
	}

	/// �����p�e�L�X�g�̕`��
	SetFontSize(100);
	GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, "�{�^�������N���b�N����ƁA�F��I���ł����",
											   strlen("�{�^�������N���b�N����ƁA�F��I���ł����"));
	DrawString(size.x / 2 - _strSize.x / 2, size.y - _strSize.y, "�{�^�������N���b�N����ƁA�F��I���ł����", 0x0000aa);

	/// �{�^���̕������W
	Vector2 pos[4];
	pos[0] = Vector2((size.x / 6),	   (size.y / 8 * 5));
	pos[1] = Vector2((size.x / 8 * 3), (size.y / 8 * 5));
	pos[2] = Vector2((size.x / 7 * 4), (size.y / 8 * 5));
	pos[3] = Vector2((size.x / 9 * 7), (size.y / 8 * 5));

	/// �{�^���̕����̕`��
	SetFontSize(120);
	int colorID = 0;
	auto type = _btnType.begin();
	for (; type != _btnType.end(); ++type)
	{
		/// �{�^���̕����ʒu�̎擾
		auto cnt = type - _btnType.begin();
		/// �{�^���̐F�����擾���Ă���
		colorID = static_cast<int>((*type));

		GetDrawStringSize(&_strSize.x, &_strSize.y, nullptr, _texts[colorID].c_str(), strlen(_texts[colorID].c_str()));
		DrawString(pos[cnt].x - _strSize.x / 2, pos[cnt].y - _strSize.y / 2, _texts[colorID].c_str(), 0x000000);
	}

	Vector2 imgSize;
	if (CheckSoundMem(_correctSE))
	{
		/// �������̕`��
		GetGraphSize(_correctImg, &imgSize.x, &imgSize.y);
		DrawGraph(size.x / 2 - imgSize.x / 2, size.y / 2 - imgSize.y / 2, _correctImg, true);
	}

	if (CheckSoundMem(_missSE))
	{
		/// �s�������̕`��
		GetGraphSize(_missImg, &imgSize.x, &imgSize.y);
		DrawRotaGraph(size.x - imgSize.x / 2, size.y - imgSize.y / 2, 0.6, 0, _missImg, true);
	}
}