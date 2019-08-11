#pragma once
#include "Scene.h"

class PauseScene : public Scene
{
private:
	void (PauseScene::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

public:
	PauseScene();
	~PauseScene();

	void Update(const Peripheral& p);
	void Draw();
};

