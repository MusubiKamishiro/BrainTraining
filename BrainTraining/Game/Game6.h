#pragma once

#include <vector>
#include <array>
#include "../Geometry.h"
#include "../Scene/Scene.h"

class Button;

enum class COLOR
{
	RED,
	BLUE,
	YELLOW,
	GREEN,
	MAX
};

#define COLOR_SIZE (static_cast<int>(COLOR::MAX))

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
	void ExpUpdate(const Peripheral& p);
	void CntDownUpdate(const Peripheral& p);
	void GameUpdate(const Peripheral& p);

	void ChangeButton();
	void ChangeColor();							// ï∂ÇÃïœçX
	void ButtonUpdater(const Peripheral& p);
	
	void StartDraw();
	void ExpDraw();
	void CntDownDraw();
	void GameDraw();

	std::array<std::shared_ptr<Button>, COLOR_SIZE> _buttons;
	std::array<std::string, COLOR_SIZE> _texts;
	std::vector<COLOR> _btnType;
	std::array<int, COLOR_SIZE> _colors;
	std::array<int, 3> _expImgs;

	Vector2 _strSize;			

	int _correctImg, _missImg;
	int _textNum, _colorNum;
	int _questions;
	int _corrects;			/// ê≥âêî
	int _timeCnt;
	int _expCnt;
	int _blindCnt;
	int _correctSE, _missSE, _cntDownSE, _startSE;
	int _gameBGM;

	bool _isColor;

	const Size _btnSize;

public:
	Game6();
	~Game6();

	void Update(const Peripheral& p);
	void Draw();

};