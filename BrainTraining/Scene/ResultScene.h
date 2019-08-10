#pragma once
#include "Scene.h"

class ResultScene : public Scene
{
private:
	void (ResultScene::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

public:
	ResultScene(const unsigned int& score, const unsigned int& cnum);
	~ResultScene();

	void Update(const Peripheral& p);
	void Draw();
};

