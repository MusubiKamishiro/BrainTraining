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
	
	std::pair<bool, bool> _judgeFlag;		// first : ��, second : ��
	std::pair<bool, bool> _plFlag;			// first : ��, second : ��

	std::string _orderText;

	bool _isJudge;			// true : ����I��, false : ���蒆
	int _moveFlagCnt;		// ���𓮂�����	
	int _lastNum;			// �Ō�ɋ��߂������̒l
	int _questions;
	int _corrects;			// ����
	int _timeCnt;


	// ���g���ްт̉摜�����(vector�ŊǗ�����悤�ɂ���)
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

