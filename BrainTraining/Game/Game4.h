#pragma once
#include "../Scene/Scene.h"

class Game4 :
	public Scene
{
private:
	void (Game4::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

public:
	Game4();
	~Game4();

	void Update(const Peripheral& p);
	void Draw();
};

