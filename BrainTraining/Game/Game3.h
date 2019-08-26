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

	void MoveJudgeFlag(const int& num);
	void MovePlFlag(const BUTTON& btn);

	void ButtonUpdater(const Peripheral& p);

	std::vector<std::string> _texts;
	// first : 赤, second : 白
	std::pair<bool, bool> _judgeFlag;
	// first : 赤, second : 白
	std::pair<bool, bool> _plFlag;

	std::string _orderText;

	bool _isJudge;			// true : 判定終了, false : 判定中
	int _moveFlagCnt;		// 旗を動かす回数	
	int _lastNum;			// 最後に求めた乱数の値
	int _questions;
	int _corrects;			// 正解数
	int _timeCnt;


	// 旗揚げｹﾞｰﾑの画像ﾊﾝﾄﾞﾙ(vectorで管理するようにする)
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

