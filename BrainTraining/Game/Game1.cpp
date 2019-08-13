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
		pal = 255;
		updater = &Game1::WaitUpdate;
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

Game1::Game1()
{
	updater = &Game1::FadeinUpdate;

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/rock.png", data);
	rock = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/paper.png", data);
	paper = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/scissors.png", data);
	scissors = data.GetHandle();

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
	DxLib::DrawString(50, 50, "å„èoÇµÇ∂Ç·ÇÒÇØÇÒÇæÇÊ", 0xffffff);

	for (int i = 0; i < buttons.size(); ++i)
	{
		buttons[i]->Draw();
	}

	DxLib::DrawExtendGraph(310, 700, 610, 1000, rock, true);
	DxLib::DrawExtendGraph(810, 700, 1110, 1000, scissors, true);
	DxLib::DrawExtendGraph(1310, 700, 1610, 1000, paper, true);

	if (count)
	{
		DxLib::DrawFormatString(100, 100, 0xff0000, "É{É^Éì[%d]ÇâüÇµÇΩÇÊ", n);
		--count;
	}
}
