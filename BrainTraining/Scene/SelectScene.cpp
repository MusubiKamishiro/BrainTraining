#include "SelectScene.h"
#include <DxLib.h>
#include "../Peripheral.h"
#include "../Game.h"
#include "SceneManager.h"
#include "GamePlayingScene.h"

#include "../Game/Game1.h"
#include "../Game/Game2.h"
#include "../Game/Game3.h"
#include "../Game/Game4.h"
#include "../Game/Game5.h"
#include "../Game/Game6.h"


void SelectScene::FadeinUpdate(const Peripheral & p)
{
	if (pal >= 255)
	{
		pal = 255;
		updater = &SelectScene::WaitUpdate;
	}
	else
	{
		pal += 20;
	}
}

void SelectScene::FadeoutUpdate(const Peripheral & p)
{
	if (pal <= 0)
	{
		//SceneManager::Instance().ChangeScene(std::make_unique <GamePlayingScene>());

		auto& manager = SceneManager::Instance();
		switch (_selectButton)
		{
		case 1:
			manager.ChangeScene(std::make_unique <Game1>());
			break;
		case 2:
			manager.ChangeScene(std::make_unique <Game2>());
			break;
		case 3:
			manager.ChangeScene(std::make_unique <Game3>());
			break;
		case 4:
			manager.ChangeScene(std::make_unique <Game4>());
			break;
		case 5:
			manager.ChangeScene(std::make_unique <Game5>());
			break;
		case 6:
			manager.ChangeScene(std::make_unique <Game6>());
			break;
		default:
			break;
		}
	}
	else
	{
		pal -= 20;
	}
}

void SelectScene::WaitUpdate(const Peripheral & p)
{
	//if (p.IsTrigger(MOUSE_INPUT_LEFT))
	//{
	//	updater = &SelectScene::FadeoutUpdate;
	//}

	auto size = Game::Instance().GetScreenSize();
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		auto pos = p.GetMousePos();
		for (int i = 0;i < 6;++i)
		{
			auto button = Vector2((size.x / 4) * (i % 3 + 1), (size.y / 3) * (i / 3 + 1));

			if (
				(pos.x - button.x) * (pos.x - button.x) + (pos.y - button.y) * (pos.y - button.y) < 120 * 120
				)
			{
				_selectButton = i + 1;
				updater = &SelectScene::FadeoutUpdate;
				break;
			}
		}
	}
}

SelectScene::SelectScene()
{
	_selectButton = 0;

	updater = &SelectScene::FadeinUpdate;
}


SelectScene::~SelectScene()
{
}

void SelectScene::Update(const Peripheral& p)
{
	(this->*updater)(p);
}

void SelectScene::Draw()
{
	auto size = Game::Instance().GetScreenSize();

	DxLib::SetFontSize(48);
	for (int i = 0;i < 6;++i)
	{
		DxLib::DrawCircle((size.x / 4) * (i % 3 + 1), (size.y / 3) * (i / 3 + 1), 120, 0xffffff, true);
		DxLib::DrawFormatString((size.x / 4) * (i % 3 + 1), (size.y / 3) * (i / 3 + 1), 0x000000, "%d", i + 1);
	}

	DxLib::SetFontSize(24);
	DxLib::DrawBox(0, 0, 100, 100, 0x00ff00, true);
	DxLib::DrawString(450, 450, "セレクトシーンだよ", 0xffffff);
}
