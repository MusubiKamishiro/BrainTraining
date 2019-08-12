#pragma once
#include "../Scene/Scene.h"

class Game3 :
	public Scene
{
private:
	void (Game3::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

public:
	Game3();
	~Game3();

	void Update(const Peripheral& p);
	void Draw();
};

