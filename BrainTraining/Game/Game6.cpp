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
	if (!CheckSoundMem(_correctSE) || !CheckSoundMem(_missSE))
	{

		if (_questions >= 20)
		{
			_updater = &Game6::FadeoutUpdate;
		}
	}

}

void Game6::StartUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
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
}

Game6::Game6()
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
	_buttons.emplace_back(new Button(Rect(size.x / 7 + 150, size.y / 4 * 3 + 75, 300, 150)));
	_buttons.emplace_back(new Button(Rect(size.x / 5 * 2 + 150, size.y / 4 * 3 + 75, 300, 150)));
	_buttons.emplace_back(new Button(Rect(size.x / 3 * 2 + 150, size.y / 4 * 3 + 75, 300, 150)));

	_correctSE = LoadSoundMem("SE/correct1.mp3");
	_missSE = LoadSoundMem("SE/incorrect1.mp3");

	_questions = _corrects = 0;

	_updater = &Game6::FadeinUpdate;
	_drawer = &Game6::StartDraw;
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
	DrawString(size.x / 2 - strWidth / 2, size.y / 2 - strHeight / 2, "�����̐F���ăQ�[��", 0x000000);
}

void Game6::GameDraw()
{
	auto size = Game::Instance().GetScreenSize();

	DxLib::DrawBox(0, 0, size.x, size.y, 0xeeeeee, true);

	int strWidth, strHeight;
	strWidth = strHeight = 0;
	SetFontSize(300);
	GetDrawStringSize(&strWidth, &strHeight, nullptr, _texts[0].c_str(), strlen(_texts[0].c_str()));
	DrawString((size.x / 2 - strWidth / 2), (size.y / 3 - strHeight / 2), _texts[0].c_str(), _colors[3]);


	SetFontSize(120);

	for (auto btn : _buttons)
	{
		btn->Draw();
	}
	/*/// ���[
	if (!_plFlag.first)
	{
		DxLib::DrawExtendGraph((size.x / 7), (size.y / 4 * 3), (size.x / 7 + 300), (size.y / 4 * 3 + 150), _upImg, true);
	}
	else
	{
		DxLib::DrawExtendGraph((size.x / 7), (size.y / 4 * 3), (size.x / 7 + 300), (size.y / 4 * 3 + 150), _downImg, true);
	}
	DxLib::DrawString(size.x / 4 * 3, size.y / 13 * 9 - strHeight / 2, "��", 0xffffff);
	/// �E�[
	if (!_plFlag.second)
	{
		DxLib::DrawExtendGraph((size.x / 3 * 2), (size.y / 4 * 3), (size.x / 3 * 2 + 300), (size.y / 4 * 3 + 150), _upImg, true);
	}
	else
	{
		DxLib::DrawExtendGraph((size.x / 3 * 2), (size.y / 4 * 3), (size.x / 3 * 2 + 300), (size.y / 4 * 3 + 150), _downImg, true);
	}
	/// �^��
	DxLib::DrawExtendGraph((size.x / 5 * 2), (size.y / 4 * 3), (size.x / 5 * 2 + 300), (size.y / 4 * 3 + 150), _stayImg, true);*/
}