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
	//DxLib::ChangeWindowMode(true);	// フルスクリーンではない
	
	DxLib::SetGraphMode(screenSize.x, screenSize.y, 32);
	DxLib::ChangeWindowMode(true);

	if (DxLib::DxLib_Init() == -1)
	{
		return;
	}

	DxLib::SetMouseDispFlag(true);	// マウスカーソルを表示する

	DxLib::SetMainWindowText("BrainTraining");	// タイトル
	//DxLib::SetWindowIconID(IDI_ICON1);		// アイコン
	DxLib::SetDrawScreen(DX_SCREEN_BACK);		// 裏画面に描画

	/// ﾌｫﾝﾄの読み込み
	if (AddFontResourceEx("Font/ほのかアンティーク丸.ttf", FR_PRIVATE, NULL) > 0)
	{
	}
	else
	{
		MessageBox(NULL, "フォント読込み失敗", "", MB_OK);
	}

	// ChangeFont("ほのかアンティーク丸", DX_CHARSET_DEFAULT);

	//AddFontResourceEx("Ronde-B_square.otf", FR_PRIVATE, nullptr);
	//DxLib::ChangeFont("ロンド B スクエア", DX_CHARSET_DEFAULT);
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
			// メイン処理
			ff.AdjustmentFrameLate();

			DxLib::ClearDrawScreen();

			// エスケープキーで終了
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

	/// ﾌｫﾝﾄの削除
	if (RemoveFontResourceEx("Font/ほのかアンティーク丸.ttf", FR_PRIVATE, NULL)) 
	{
	}
	else
	{
		MessageBox(NULL, "フォントは既に削除されています", "", MB_OK);
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
