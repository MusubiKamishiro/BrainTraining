#pragma once
#include "../Scene/Scene.h"

class Game1 :
	public Scene
{
private:
	void (Game1::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

public:
	Game1();
	~Game1();

	void Update(const Peripheral& p);
	void Draw();
};

