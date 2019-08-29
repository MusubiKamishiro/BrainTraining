#pragma once
#include "Scene.h"

class ResultScene : public Scene
{
private:
	int _questions;
	int _correct;

	int _frame;

	int _img_back;
	int _img_rankS;
	int _img_rankA;
	int _img_rankB;
	int _img_rankC;
	int _img_rankD;

	void (ResultScene::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);
public:
	ResultScene(int questions = 0, int correct = 0);
	~ResultScene();

	void Update(const Peripheral& p);
	void Draw();
};

