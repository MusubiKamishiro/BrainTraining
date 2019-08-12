#include "PauseScene.h"
#include <DxLib.h>
#include "../Peripheral.h"
#include "../Game.h"
#include "SceneManager.h"


void PauseScene::FadeinUpdate(const Peripheral & p)
{
	updater = &PauseScene::WaitUpdate;
}

void PauseScene::FadeoutUpdate(const Peripheral & p)
{
	SceneManager::Instance().PopScene();
}

void PauseScene::WaitUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		updater = &PauseScene::FadeoutUpdate;
	}
}

PauseScene::PauseScene()
{
	pal = 255;
	updater = &PauseScene::FadeinUpdate;
}


PauseScene::~PauseScene()
{
}

void PauseScene::Update(const Peripheral& p)
{
	(this->*updater)(p);
}

void PauseScene::Draw()
{
	DxLib::DrawString(500, 500, "É|Å[ÉYíÜÇæÇÊ", 0xffffff);
}
