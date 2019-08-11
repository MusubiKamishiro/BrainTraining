#include "Game.h"
#include <DxLib.h>
#include "Peripheral.h"
#include "Scene/SceneManager.h"
#include "FrameFixity.h"


Game::Game() : ScreenSize(1600, 900)
{
	time = 0.0;
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
#ifdef _DEBUG
	DxLib::ChangeWindowMode(true);
#else
	int ans = MessageBox(DxLib::GetMainWindowHandle(), "フルスクリーンで表示しますか？", "画面の大きさどうしようか", MB_YESNO | MB_ICONQUESTION);

	if (ans == IDYES)
	{
		DxLib::ChangeWindowMode(false);
	}
	else
	{
		DxLib::ChangeWindowMode(true);
	}
#endif // DEBUG

	DxLib::SetGraphMode(ScreenSize.x, ScreenSize.y, 32);

	if (DxLib::DxLib_Init() == -1)
	{
		return;
	}

	DxLib::SetMainWindowText("BrainTraining");	// タイトル
	//DxLib::SetWindowIconID(IDI_ICON1);		// アイコン
	DxLib::SetDrawScreen(DX_SCREEN_BACK);		// 裏画面に描画

	//AddFontResourceEx("Ronde-B_square.otf", FR_PRIVATE, nullptr);
	//DxLib::ChangeFont("ロンド B スクエア", DX_CHARSET_DEFAULT);
	DxLib::SetFontSize(fontSize);
}

void Game::Run()
{
	Peripheral peripheral;
	auto& scenes = SceneManager::Instance();
	FrameFixity& ff = FrameFixity::Instance();
	ff.FFInitialize();

	while (DxLib::ProcessMessage() == 0)
	{
		if (ff.CheckReceiveMessage()) {
			if (ff.GetReceiveMessage().message == WM_QUIT) {
				break;
			}
		}
		else {
			ff.AdjustmentFrameLate();

			++time;
			DxLib::ClearDrawScreen();

			// エスケープキーで終了
			if (DxLib::CheckHitKey(KEY_INPUT_ESCAPE))
			{
				break;
			}

			peripheral.Update();
			scenes.Update(peripheral);
			scenes.Draw();

			DxLib::ScreenFlip();
		}
	}

	ff.Terminate();
}

void Game::Terminate()
{
	DxLib::DxLib_End();
}

const Vector2& Game::GetScreenSize()const
{
	return ScreenSize;
}
