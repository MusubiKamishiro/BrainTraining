#pragma once
#include "Scene.h"

class ResultScene : public Scene
{
private:
	void (ResultScene::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	int _questions;
	int _correct;

public:
	ResultScene(int questions = 0, int correct = 0);
	~ResultScene();

	void Update(const Peripheral& p);
	void Draw();
};

