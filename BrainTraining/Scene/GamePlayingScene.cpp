#include "GamePlayingScene.h"
#include <DxLib.h>


#include "../Peripheral.h"
#include "../Game.h"
#include "SceneManager.h"
#include "ResultScene.h"
#include "TitleScene.h"

void GamePlayingScene::FadeinUpdate(const Peripheral & p)
{
	if (pal > 255)
	{
		pal = 255;
		updater = &GamePlayingScene::WaitUpdate;
	}
	else
	{
		pal += 20;
	}
}

void GamePlayingScene::FadeoutUpdate(const Peripheral & p)
{
	if (pal <= 0)
	{
		SceneManager::Instance().ChangeScene(std::make_unique<TitleScene>());
	}
	else
	{
		pal -= 20;
	}
}

void GamePlayingScene::WaitUpdate(const Peripheral & p)
{
}

GamePlayingScene::GamePlayingScene()
{
	updater = &GamePlayingScene::FadeinUpdate;
}


GamePlayingScene::~GamePlayingScene()
{
}

void GamePlayingScene::Update(const Peripheral& p)
{
	(this->*updater)(p);
}

void GamePlayingScene::Draw()
{
}
