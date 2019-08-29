#include "PauseScene.h"
#include <DxLib.h>
#include "../Peripheral.h"
#include "../Game.h"
#include "SceneManager.h"

#include "SelectScene.h"

#include "../Button.h"

#include "../System/FileSystem.h"
#include "../System/ImageLoader.h"
#include "../System/SoundLoader.h"

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

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/Button/red.png", data);
	buttonImg = data.GetHandle();

	buttons.emplace_back(new Button(Rect(ssize.x/2, 400, 1000, 200), buttonImg));
	buttons.emplace_back(new Button(Rect(ssize.x/2, 800, 1000, 200), buttonImg));

	buttonStatements[0] = "ゲームに戻る";
	buttonStatements[1] = "ゲームセレクトに戻る";
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
	auto size = Game::Instance().GetScreenSize();
	DrawBox(size.x / 2 - 500, 0, size.x / 2 + 500, size.y, 0x00ffff, true);

	DxLib::SetFontSize(96);

	int strwidth, strheight;
	strwidth = strheight = 0;

	for (int i = 0; i < buttons.size(); ++i)
	{
		std::string s = buttonStatements[i];
		auto rect = buttons[i]->GetButtonRect();

		buttons[i]->Draw();

		GetDrawStringSize(&strwidth, &strheight, nullptr, s.c_str(), strlen(s.c_str()));
		DrawFormatString(rect.center.x - strwidth / 2, rect.center.y - strheight / 2, 0xffffff, s.c_str());
	}
}
