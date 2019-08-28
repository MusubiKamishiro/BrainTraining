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
	void ExpUpdate(const Peripheral& p);
	void CntDownUpdate(const Peripheral& p);

	void StartDraw();
	void ExpDraw();
	void CntDownDraw();
	void GameDraw();

	void MoveJudgeFlag(const int& num, const FLAG& btn);
	void MovePlFlag(const FLAG& btn);

	void JudgeFlagUpdater();
	void ButtonUpdater(const Peripheral& p);

	std::vector<std::shared_ptr<Button>> _buttons;
	std::vector<std::string> _texts;
	std::vector<int> _flagImgs;			// 旗上げｷｬﾗｸﾀｰの画像ﾊﾝﾄﾞﾙ取得用
	std::vector<int> _expImgs;			// 説明用画像のﾊﾝﾄﾞﾙ取得用
	
	std::pair<bool, bool> _judgeFlag;	// first : 赤, second : 白
	std::pair<bool, bool> _plFlag;		// first : 赤, second : 白

	std::string _orderText;

	bool _isJudge;			// true : 判定終了, false : 判定中
	int _moveFlagCnt;		// 旗を動かす回数	
	int _lastNum;			// 最後に求めた乱数の値
	int _questions;
	int _corrects;			// 正解数
	int _timeCnt;
	int _expCnt;			// 説明用のｶｳﾝﾄ
	int _blindCnt;			// 点滅用のｶｳﾝﾄ

	int _correctSE, _missSE, _cntDownSE, _startSE;

	const int _defTime;
public:
	Game3();
	~Game3();

	void Update(const Peripheral& p);
	void Draw();

};

