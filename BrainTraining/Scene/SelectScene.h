#pragma once
#include "Scene.h"
#include "../Geometry.h"

#include <vector>

struct Menu
{
	Vector2 pos;
	int handle;
	bool on;
};

class SelectScene : public Scene
{
private:
	void (SelectScene::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	int _onButton;	// �|�C���^�̏���Ă���{�^���ԍ�
	int _selectButton;	// �Q�[���ԍ�

	int _img_kokuban;
	int _bgm;

	std::vector<Menu> _menus;

public:
	SelectScene();
	~SelectScene();

	void Update(const Peripheral& p);
	void Draw();
};

