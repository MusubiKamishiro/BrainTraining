#pragma once
#include <vector>
#include "../Geometry.h"
#include "../Scene/Scene.h"

class Button;

enum class JFLAG
{
	UP,
	DOWN,
	STAYUP,
	STAYDOWN,
	MAX
};

enum class BUTTON
{
	RED,
	STAY,
	WHITE,
	MAX
};

class Game3 :
	public Scene
{
private:
	void (Game3::*_updater)(const Peripheral& p);
	void (Game3::*_drawer)();

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);
	void StartUpdate(const Peripheral& p);
	void FirstUpdate(const Peripheral& p);
	void GameUpdate(const Peripheral& p);

	void StartDraw();
	void GameDraw();

	bool ChangeJudgeFlag(const int& num);
	void ChangeFlag(const BUTTON& btn);

	void ButtonUpdater(const Peripheral& p);

	std::vector<std::string> _texts;
	// first : Τ, second : 
	std::pair<bool, bool> _judgeFlag;
	// first : Τ, second : 
	std::pair<bool, bool> _plFlag;

	bool _isJudge;			/// true : »θIΉ, false : »θ
	int _lastNum;			/// ΕγΙί½Μl
	int _questions;
	int _corrects;			/// ³π


	// ψg°Ήή°ΡΜζΚέΔήΩ
	int _upImg, _downImg, _stayImg, _flagImg, _flag2Img, _flag3Img, _flag4Img;
	int _correctSE, _missSE;

	const Size _btnSize;

	std::vector<std::shared_ptr<Button>> _buttons;

public:
	Game3();
	~Game3();

	void Update(const Peripheral& p);
	void Draw();

};

