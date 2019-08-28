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
	std::vector<int> _flagImgs;			// ���グ��׸���̉摜����َ擾�p
	std::vector<int> _expImgs;			// �����p�摜������َ擾�p
	
	std::pair<bool, bool> _judgeFlag;	// first : ��, second : ��
	std::pair<bool, bool> _plFlag;		// first : ��, second : ��

	std::string _orderText;

	bool _isJudge;			// true : ����I��, false : ���蒆
	int _moveFlagCnt;		// ���𓮂�����	
	int _lastNum;			// �Ō�ɋ��߂������̒l
	int _questions;
	int _corrects;			// ����
	int _timeCnt;
	int _expCnt;			// �����p�̶���
	int _blindCnt;			// �_�ŗp�̶���

	int _correctSE, _missSE, _cntDownSE, _startSE;

	const int _defTime;
public:
	Game3();
	~Game3();

	void Update(const Peripheral& p);
	void Draw();

};

