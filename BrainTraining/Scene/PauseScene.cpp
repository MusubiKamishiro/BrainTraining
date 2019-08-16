#include "PauseScene.h"
#include <DxLib.h>
#include "../Peripheral.h"
#include "../Game.h"
#include "SceneManager.h"

#include "SelectScene.h"

#include "../Button.h"


void PauseScene::FadeinUpdate(const Peripheral & p)
{
	updater = &PauseScene::WaitUpdate;
}

void PauseScene::FadeoutUpdate(const Peripheral & p)
{
	switch (select)
	{
	case 0:
		SceneManager::Instance().PopScene();
		break;
	case 1:
		///
		break;
	case 2:
		if (pal <= 0)
		{
			SceneManager::Instance().ChangeScene(std::make_unique<SelectScene>());
		}
		else
		{
			pal -= 20;
		}
		break;
	default:
		break;
	}
}

void PauseScene::WaitUpdate(const Peripheral & p)
{
	for (unsigned int i = 0; i < buttons.size(); ++i)
	{
		if (buttons[i]->Update(p))
		{
			select = i;
			updater = &PauseScene::FadeoutUpdate;
		}
	}
}

PauseScene::PauseScene()
{
	pal = 255;
	updater = &PauseScene::FadeinUpdate;

	auto ssize = Game::Instance().GetScreenSize();

	buttons.emplace_back(new Button(Rect(ssize.x/2, 300, 1000, 200)));
	buttons.emplace_back(new Button(Rect(ssize.x/2, 600, 1000, 200)));
	buttons.emplace_back(new Button(Rect(ssize.x/2, 900, 1000, 200)));

	buttonStatements[0] = "ゲームに戻る";
	buttonStatements[1] = "ゲームをやり直す";
	buttonStatements[2] = "ゲームセレクトに戻る";
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
	DxLib::DrawString(500, 500, "ポーズ中だよ", 0xffffff);
	for (int i = 0; i < buttons.size(); ++i)
	{
		std::string s = buttonStatements[i];
		auto rect = buttons[i]->GetButtonRect();

		buttons[i]->Draw();
		DxLib::DrawFormatString(rect.Left(), rect.Top(), 0xffffff, "%s", s.c_str());
	}
}
