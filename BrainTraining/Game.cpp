#include "Game.h"
#include <DxLib.h>
#include "Peripheral.h"
#include "Scene/SceneManager.h"
#include "FrameFixity.h"
#include "System/FileSystem.h"


Game::Game() : screenSize(1920, 1080)
{
	fontSize = 24;
}

void Game::operator=(const Game &)
{
}

Game::~Game()
{
}

void Game::Initialize()
{
	//DxLib::ChangeWindowMode(true);	// �t���X�N���[���ł͂Ȃ�
	
	DxLib::SetGraphMode(screenSize.x, screenSize.y, 32);
	DxLib::ChangeWindowMode(true);

	if (DxLib::DxLib_Init() == -1)
	{
		return;
	}

	DxLib::SetMouseDispFlag(true);	// �}�E�X�J�[�\����\������

	DxLib::SetMainWindowText("BrainTraining");	// �^�C�g��
	//DxLib::SetWindowIconID(IDI_ICON1);		// �A�C�R��
	DxLib::SetDrawScreen(DX_SCREEN_BACK);		// ����ʂɕ`��

	/// ̫�Ă̓ǂݍ���
	if (AddFontResourceEx("Font/�ق̂��A���e�B�[�N��.ttf", FR_PRIVATE, NULL) > 0)
	{
	}
	else
	{
		MessageBox(NULL, "�t�H���g�Ǎ��ݎ��s", "", MB_OK);
	}

	// ChangeFont("�ق̂��A���e�B�[�N��", DX_CHARSET_DEFAULT);

	//AddFontResourceEx("Ronde-B_square.otf", FR_PRIVATE, nullptr);
	//DxLib::ChangeFont("�����h B �X�N�G�A", DX_CHARSET_DEFAULT);
	DxLib::SetFontSize(fontSize);

	fileSystem.reset(new FileSystem());
}

void Game::Run()
{
	Peripheral peripheral;
	auto& scenes = SceneManager::Instance();
	FrameFixity& ff = FrameFixity::Instance();
	ff.FFInitialize();

	while (DxLib::ProcessMessage() == 0)
	{
		if (ff.CheckReceiveMessage())
		{
			if (ff.GetReceiveMessage().message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			// ���C������
			ff.AdjustmentFrameLate();

			DxLib::ClearDrawScreen();

			// �G�X�P�[�v�L�[�ŏI��
			if (DxLib::CheckHitKey(KEY_INPUT_ESCAPE))
			{
				break;
			}

			peripheral.Update();
			scenes.Update(peripheral);
			scenes.Draw();

#ifdef _DEBUG
			peripheral.DebugDraw();
#endif // _DEBUG

			DxLib::ScreenFlip();
		}
	}

	/// ̫�Ă̍폜
	if (RemoveFontResourceEx("Font/�ق̂��A���e�B�[�N��.ttf", FR_PRIVATE, NULL)) 
	{
	}
	else
	{
		MessageBox(NULL, "�t�H���g�͊��ɍ폜����Ă��܂�", "", MB_OK);
	}

	ff.Terminate();
}

void Game::Terminate()
{
	DxLib::DxLib_End();
}

const Vector2& Game::GetScreenSize()const
{
	return screenSize;
}

const std::shared_ptr<FileSystem> Game::GetFileSystem() const
{
	return fileSystem;
}
