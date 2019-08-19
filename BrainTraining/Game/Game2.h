#pragma once
#include "../Scene/Scene.h"

class Game2 :
	public Scene
{
private:
	int _right;	// 右の値
	int _left;	// 左の値

	int _questions;		// 問題番号
	int _correctNum;	// 正解数

	int _SE_question;	// 出題SE
	int _SE_correct;	// 正解SE
	int _SE_miss;		// はずれSE

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

public:
	Game2();
	~Game2();

	void Update(const Peripheral& p);
	void Draw();
};

