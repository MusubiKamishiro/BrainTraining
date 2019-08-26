#pragma once
#include <vector>
#include "../Geometry.h"
#include "../Scene/Scene.h"

class Button;

enum class JFLAG
{
	UP,
	DOWN,
	MAX
};

enum class BUTTON
{
	RED,
	WHITE,
	STAY,
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

	void MoveJudgeFlag(const int& num, const BUTTON& btn);
	void MovePlFlag(const BUTTON& btn);

	void JuedeFlagUpdater();
	void ButtonUpdater(const Peripheral& p);

	std::vector<std::string> _texts;
	
	std::pair<bool, bool> _judgeFlag;		// first : Ô, second : ”’
	std::pair<bool, bool> _plFlag;			// first : Ô, second : ”’

	std::string _orderText;

	bool _isJudge;			// true : ”»’èI—¹, false : ”»’è’†
	int _moveFlagCnt;		// Šø‚ğ“®‚©‚·‰ñ”	
	int _lastNum;			// ÅŒã‚É‹‚ß‚½—”‚Ì’l
	int _questions;
	int _corrects;			// ³‰ğ”
	int _timeCnt;


	// Šø—g‚°¹Ş°Ñ‚Ì‰æ‘œÊİÄŞÙ(vector‚ÅŠÇ—‚·‚é‚æ‚¤‚É‚·‚é)
	int _upImg, _downImg, _stayImg, _flagImg, _flag2Img, _flag3Img, _flag4Img;
	int _correctSE, _missSE;

	const Size _btnSize;
	const int _defTime;

	std::vector<std::shared_ptr<Button>> _buttons;

public:
	Game3();
	~Game3();

	void Update(const Peripheral& p);
	void Draw();

};

