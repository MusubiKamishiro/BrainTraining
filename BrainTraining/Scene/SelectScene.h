#pragma once
#include "Scene.h"


class SelectScene : public Scene
{
private:
	void (SelectScene::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	int _onButton;	// �|�C���^�̏���Ă���{�^���ԍ�
	int _selectButton;	// �Q�[���ԍ�

public:
	SelectScene();
	~SelectScene();

	void Update(const Peripheral& p);
	void Draw();
};

