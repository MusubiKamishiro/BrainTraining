#pragma once
#include <vector>
#include "../Scene/Scene.h"

class Button;

class Game6 :
	public Scene
{
private:
	void (Game6::*_updater)(const Peripheral& p);
	void (Game6::*_drawer)();

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);
	void StartUpdate(const Peripheral& p);
	void GameUpdate(const Peripheral& p);

	void StartDraw();
	void GameDraw();

	int _questions;
	int _corrects;			/// ê≥âêî

	int _correctSE, _missSE;

	std::vector<std::shared_ptr<Button>> _buttons;

public:
	Game6();
	~Game6();

	void Update(const Peripheral& p);
	void Draw();

};

