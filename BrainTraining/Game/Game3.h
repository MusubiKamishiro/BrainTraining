#pragma once
#include <vector>
#include "../Scene/Scene.h"

enum class JFLAG
{
	UP,
	DOWN,
	STAYUP,
	STAYDOWN,
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

	bool ChangeJudgeFlag(const int& num);

	std::vector<std::string> _texts;
	// first : ê‘, second : îí
	std::pair<bool, bool> _judgeFlag;
	// first : ê‘, second : îí
	std::vector<std::pair<bool, bool>> _flag;

	int _lastNum;

public:
	Game3();
	~Game3();

	void Update(const Peripheral& p);
	void Draw();
};

