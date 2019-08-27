#pragma once
#include <vector>
#include "../Geometry.h"
#include "../Scene/Scene.h"

class Button;

enum class ORDER
{
	UP,
	DOWN,
	MAX
};

enum class FLAG
{
	RED,
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

	void MoveJudgeFlag(const int& num, const FLAG& btn);
	void MovePlFlag(const FLAG& btn);

	void JudgeFlagUpdater();
	void ButtonUpdater(const Peripheral& p);

	std::vector<std::shared_ptr<Button>> _buttons;
	std::vector<std::string> _texts;
	std::vector<int> _flagImgs;			// Šøã‚°·¬×¸À°‚Ì‰æ‘œÊİÄŞÙæ“¾—p
	
	std::pair<bool, bool> _judgeFlag;	// first : Ô, second : ”’
	std::pair<bool, bool> _plFlag;		// first : Ô, second : ”’

	std::string _orderText;

	bool _isJudge;			// true : ”»’èI—¹, false : ”»’è’†
	int _moveFlagCnt;		// Šø‚ğ“®‚©‚·‰ñ”	
	int _lastNum;			// ÅŒã‚É‹‚ß‚½—”‚Ì’l
	int _questions;
	int _corrects;			// ³‰ğ”
	int _timeCnt;

	int _correctSE, _missSE;

	const int _defTime;
public:
	Game3();
	~Game3();

	void Update(const Peripheral& p);
	void Draw();

};

