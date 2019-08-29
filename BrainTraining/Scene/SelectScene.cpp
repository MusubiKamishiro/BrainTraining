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

constexpr int menuNum = 5;

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
	auto size = Game::Instance().GetScreenSize();
	auto pos = p.GetMousePos();
	for (int i = 0; i < menuNum; ++i)
	{
		auto button = _menus[i].pos;

		if (
			_menus[i].pos.x - 200 < pos.x && _menus[i].pos.y - 150 < pos.y &&
			_menus[i].pos.x + 200 > pos.x && _menus[i].pos.y + 150 > pos.y
			)
		{
			_menus[i].on = true;
			if (p.IsTrigger(MOUSE_INPUT_LEFT))
			{
				StopSoundMem(_bgm);
				PlaySoundMem(_se, DX_PLAYTYPE_BACK);
				_selectButton = i + 1;
				updater = &SelectScene::FadeoutUpdate;
				break;
			}
		}
		else
		{
			_menus[i].on = false;
		}
	}
}

SelectScene::SelectScene()
{
	_selectButton = 0;

	_img_kokuban = LoadGraph("img/Title/kokuban.png");
	_bgm = LoadSoundMem("BGM/select.mp3");
	_se = LoadSoundMem("SE/decide.mp3");

	auto size = Game::Instance().GetScreenSize();

	_menus.resize(menuNum);
	for (int i = 0; i < menuNum; ++i)
	{
		_menus[i].pos = Vector2((size.x / 4) * (i % 3 + 1) + size.x / 8 * (i / 3), (size.y / 3) * (i / 3 + 1));
	}
	_menus[0].handle = LoadGraph("img/Title/janken.png");
	_menus[1].handle = LoadGraph("img/Title/suuji.png");
	_menus[2].handle = LoadGraph("img/Title/hataage.png");
	_menus[3].handle = LoadGraph("img/Title/hiragana.png");
	_menus[4].handle = LoadGraph("img/Title/iroate.png");

	PlaySoundMem(_bgm,DX_PLAYTYPE_BACK);
	updater = &SelectScene::FadeinUpdate;
}


SelectScene::~SelectScene()
{
	DeleteGraph(_img_kokuban);
	for (auto& m : _menus)
	{
		DeleteGraph(m.handle);
	}
	DeleteSoundMem(_bgm);
	DeleteSoundMem(_se);
}

void SelectScene::Update(const Peripheral& p)
{
	(this->*updater)(p);
}

void SelectScene::Draw()
{
	auto size = Game::Instance().GetScreenSize();

	DrawBox(0, 0, size.x, size.y, 0xffffff, true);
	DrawRotaGraph(size.x / 2, size.y / 2, 1.3, 0.0, _img_kokuban, true);

	for (auto& m : _menus)
	{
		DrawRotaGraph(m.pos.x, m.pos.y, m.on ? 0.32 : 0.3, 0.0, m.handle, true);
	}

	int strwidth, strheight;
	SetFontSize(100);
	GetDrawStringSize(&strwidth, &strheight, nullptr, "遊びたいゲームをクリックしてね!", strlen("遊びたいゲームをクリックしてね!"));
	DrawString(size.x / 2 - strwidth / 2, strheight / 3, "遊びたいゲームをクリックしてね!", 0xdddd00);
}
