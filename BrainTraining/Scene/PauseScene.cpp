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

	buttonStatements[0] = "ƒQ[ƒ€‚É–ß‚é";
	buttonStatements[1] = "ƒQ[ƒ€‚ð‚â‚è’¼‚·";
	buttonStatements[2] = "ƒQ[ƒ€ƒZƒŒƒNƒg‚É–ß‚é";
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
