#pragma once
#include "../Scene/Scene.h"

#include <vector>
#include "../Geometry.h"

enum class DrawType
{
	Number,
	Money,
};

struct Num
{
	int money_1;
	int money_5;
	int money_10;
	int money_50;
	int money_100;
	int money_500;
	int money_1000;
	int total;
	DrawType type;
};

class Game2 :
	public Scene
{
private:

	std::vector<Vector2> _positions;

	Num _right;	// �E�̒l
	Num _left;	// ���̒l

	void NumInit(Num &num);
	void Total(Num &num);

	int _questions;		// ���ԍ�
	int _correctNum;	// ����

	int _SE_question;	// �o��SE
	int _SE_correct;	// ����SE
	int _SE_miss;		// �͂���SE

	int _img_1en;
	int _img_5en;
	int _img_10en;
	int _img_50en;
	int _img_100en;
	int _img_500en;
	int _img_1000en;
	int _img_5000en;
	int _img_10000en;

	void (Game2::*_updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);

	void StartUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);
	void QuestionsUpdate(const Peripheral& p);
	void AnswerUpdate(const Peripheral& p);

	void (Game2::*_drawer)();

	void StartDraw();
	void WaitDraw();
	void QuestionsDraw();
	void AnswerDraw();

	void DrawMoney(Num num, int offset = 0);

public:
	Game2();
	~Game2();

	void Update(const Peripheral& p);
	void Draw();
};

