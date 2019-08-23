#pragma once
#include <vector>
#include "../Geometry.h"
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

	std::vector<std::string> _texts;
	std::vector<int> _colors;

	void StartDraw();
	void GameDraw();

	int _textNum, _colorNum;
	int _questions;
	int _corrects;			/// ����

	int _correctSE, _missSE;

	bool _isColor;

	const Size _btnSize;

	std::vector<std::shared_ptr<Button>> _buttons;

public:
	Game6();
	~Game6();

	void Update(const Peripheral& p);
	void Draw();

};

