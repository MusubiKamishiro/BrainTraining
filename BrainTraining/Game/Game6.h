#pragma once
#include "../Scene/Scene.h"

class Game6 :
	public Scene
{
private:
	void (Game6::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

public:
	Game6();
	~Game6();

	void Update(const Peripheral& p);
	void Draw();
};

