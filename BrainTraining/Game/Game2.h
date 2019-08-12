#pragma once
#include "../Scene/Scene.h"

class Game2 :
	public Scene
{
private:
	void (Game2::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

public:
	Game2();
	~Game2();

	void Update(const Peripheral& p);
	void Draw();
};

