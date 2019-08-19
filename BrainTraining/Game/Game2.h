#pragma once
#include "../Scene/Scene.h"

class Game2 :
	public Scene
{
private:
	int _right;	// �E�̒l
	int _left;	// ���̒l

	int _questions;		// ���ԍ�
	int _correctNum;	// ����

	int _SE_question;	// �o��SE
	int _SE_correct;	// ����SE
	int _SE_miss;		// �͂���SE

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

