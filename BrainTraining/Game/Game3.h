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
	void (Game3::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);
	void StartUpdate(const Peripheral& p);
	void JudgeUpdate(const Peripheral& p);

	bool ChangeJudgeFlag(const int& num);
	void ChangeFlag(const BUTTON& btn);

	std::vector<std::string> _texts;
	// first : Ô, second : ”’
	std::pair<bool, bool> _judgeFlag;
	// first : Ô, second : ”’
	std::pair<bool, bool> _plFlag;

	bool _isJudge;
	int _lastNum;
	// Šø—g‚°¹Ş°Ñ‚Ì‰æ‘œÊİÄŞÙ
	int _upImg, _downImg, _stayImg;

	/// debug—p
	int cnt = 0;
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

