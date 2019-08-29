#pragma once
#include "Scene.h"
#include <memory>
#include "../Geometry.h"

class Button;

class TitleScene : public Scene
{
private:
	void (TitleScene::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	bool flag;

	int _img_flag;
	int  _img_kokuban;
	int _bgm;
	int _se;
	int _blindCnt;
	int _button;

	std::shared_ptr<Button> button;

public:
	TitleScene();
	~TitleScene();

	void Update(const Peripheral& p);
	void Draw();
};

