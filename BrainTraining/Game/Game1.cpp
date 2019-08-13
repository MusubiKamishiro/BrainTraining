#include "Game1.h"

#include <DxLib.h>

#include "../Peripheral.h"
#include "../Game.h"
#include "../Button.h"
#include "../Geometry.h"

#include "../Scene/SceneManager.h"
#include "../Scene/ResultScene.h"
#include "../Scene/PauseScene.h"

#include "../System/FileSystem.h"
#include "../System/ImageLoader.h"



void Game1::FadeinUpdate(const Peripheral & p)
{
	if (pal > 255)
	{
		if (p.IsTrigger(MOUSE_INPUT_LEFT))
		{
			pal = 255;
			updater = &Game1::DescriptionUpdate;
			drawer = &Game1::DescriptionDraw;
		}
	}
	else
	{
		pal += 20;
	}
}

void Game1::FadeoutUpdate(const Peripheral & p)
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

void Game1::WaitUpdate(const Peripheral & p)
{
	/*if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		pal = 255;
		updater = &Game1::FadeoutUpdate;
	}*/

	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}
}

void Game1::DescriptionUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		updater = &Game1::GameUpdate;
		drawer = &Game1::GameDraw;
	}
}

void Game1::GameUpdate(const Peripheral & p)
{
}

void Game1::TitleDraw()
{
	DxLib::DrawString(400, 50, "後出しじゃんけんだよ", 0xffffff);
}

void Game1::DescriptionDraw()
{
	DxLib::DrawString(400, 50, "ルール説明だよ", 0xffffff);
}

void Game1::GameDraw()
{
	// グーちょきぱーボタンの描画
	for (int i = 0; i < buttons.size(); ++i)
	{
		buttons[i]->Draw();
	}
	DxLib::DrawExtendGraph(310, 700, 610, 1000, rock, true);
	DxLib::DrawExtendGraph(810, 700, 1110, 1000, scissors, true);
	DxLib::DrawExtendGraph(1310, 700, 1610, 1000, paper, true);

	// 問題文章の描画
	DxLib::DrawString(700, 100, "勝ってください", 0xffffff);

	// 問題の手
	DxLib::DrawExtendGraph(810, 150, 1110, 450, rock, true);
}

Game1::Game1()
{
	updater = &Game1::FadeinUpdate;
	drawer = &Game1::TitleDraw;

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/rock.png", data);
	rock = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/scissors.png", data);
	scissors = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/paper.png", data);
	paper = data.GetHandle();
	
	buttons.emplace_back(new Button(Rect(455, 850, 300, 300)));
	buttons.emplace_back(new Button(Rect(955, 850, 300, 300)));
	buttons.emplace_back(new Button(Rect(1455, 850, 300, 300)));
}


Game1::~Game1()
{
}

void Game1::Update(const Peripheral & p)
{
	for (int i = 0; i < buttons.size(); ++i)
	{
		if (buttons[i]->Update(p))
		{
			count = 60;
			n = i;
		}
	}

	(this->*updater)(p);
}

void Game1::Draw()
{
	DxLib::DrawString(50, 50, "後出しじゃんけんだよ", 0xffffff);

	if (count)
	{
		DxLib::DrawFormatString(100, 100, 0xff0000, "ボタン[%d]を押したよ", n);
		--count;
	}

	(this->*drawer)();
}
