#pragma once
#include "../Scene/Scene.h"

class Game5 :
	public Scene
{
private:
	void (Game5::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

public:
	Game5();
	~Game5();

	void Update(const Peripheral& p);
	void Draw();
};

