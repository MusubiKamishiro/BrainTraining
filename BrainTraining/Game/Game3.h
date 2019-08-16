#pragma once
#include <vector>
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

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);
	void StartUpdate(const Peripheral& p);
	void GameUpdate(const Peripheral& p);

	bool ChangeJudgeFlag(const int& num);
	void ChangeFlag(const BUTTON& btn);

	std::vector<std::string> _texts;
	// first : Ô, second : ”’
	std::pair<bool, bool> _judgeFlag;
	// first : Ô, second : ”’
	std::pair<bool, bool> _plFlag;

	bool _isJudge;			/// true : ”»’èI—¹, false : ”»’è’†
	int _lastNum;			/// ÅŒã‚É‹‚ß‚½—”‚Ì’l
	int _startCnt;

	// Šø—g‚°¹Ş°Ñ‚Ì‰æ‘œÊİÄŞÙ
	int _upImg, _downImg, _stayImg, _flagImg, _flag2Img, _flag3Img, _flag4Img;
	int _correctSE, _missSE;


	/// debug—p
	int debug = 0;
	int n = 0;

	const int _timeCnt;

	std::vector<std::shared_ptr<Button>> _buttons;

public:
	Game3();
	~Game3();

	void Update(const Peripheral& p);
	void Draw();
};

