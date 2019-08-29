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

	bool flag;

	int _img_flag;
	int  _img_kokuban;
	int _bgm;
	int _se;
	int _blindCnt;

public:
	TitleScene();
	~TitleScene();

	void Update(const Peripheral& p);
	void Draw();
};

