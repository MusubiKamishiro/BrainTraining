#pragma once
#include "Scene.h"
#include "../Geometry.h"


class TitleScene : public Scene
{
private:
	void (TitleScene::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	Vector2 ssize;

	bool flag;
	int img;

	int _img_balance;
	int _img_math;
	int _img_flag;
	int  _img_kokuban;

public:
	TitleScene();
	~TitleScene();

	void Update(const Peripheral& p);
	void Draw();
};

