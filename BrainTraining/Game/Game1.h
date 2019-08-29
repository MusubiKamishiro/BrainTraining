#pragma once
#include "../Scene/Scene.h"
#include <memory>
#include <vector>
#include <map>
#include <array>

class Button;

// ��̌`
enum class Hand
{
	ROCK,		// �O�[
	SCISSORS,	// �`���L
	PAPER,		// �p�[
	MAX
};

// ���
enum class Question
{
	WIN,		// �����Ă�������
	LOSE,		// �����Ă�������
	DONOTWIN,	// �����Ȃ��ł�������
	DONOTLOSE,	// �����Ȃ��ł�������
	MAX
};

// ����񂯂�̌���
enum class Result
{
	WIN,
	DRAW,
	LOSE,
	MAX
};

///��o������񂯂�
///LaterRPS
class Game1 :
	public Scene
{
private:
	void (Game1::*updater)(const Peripheral& p);	// �A�b�v�f�[�g�̊֐��|�C���^

	void FadeinUpdate(const Peripheral& p)override final;
	void FadeoutUpdate(const Peripheral& p)override final;
	void WaitUpdate(const Peripheral& p)override final;

	void DescriptionUpdate(const Peripheral& p);		// ���[������
	void QuestionDisplayUpdate(const Peripheral& p);	// ���\��
	void AnswerCheckUpdate(const Peripheral& p);		// �񓚊m�F
	void AnswerDisplayUpdate(const Peripheral& p);		// �񓚕\��

	void (Game1::*drawer)();	// �h���[�̊֐��|�C���^

	void TitleDraw();			// �~�j�Q�[���^�C�g���`��
	void DescriptionDraw();		// ���[�������`��
	void GameDraw();			// �Q�[�����C���`��

	void CreateQuestion();		// ���쐬
	Result JudgeResult(int& qNum, Hand& myHand);	// ����񂯂񌋉ʂ̊m�F

	std::vector<int> handImg;	// �O�[���傫�ρ[�̉摜�n���h��
	int rock, paper, scissors;	// �{�^���̉摜�n���h��
	int trueSE, falseSE;		// ����,�s�����̎��̌��ʉ�

	std::map<int, std::string> questionStatements;	// ��蕶
	std::array<int, static_cast<int>(Hand::MAX)> questionHands;	// ���̎�
	
	int nowQNum;	// ���݂̖��ԍ�

	int qStatementNum;	// ��蕶��
	int qHandNum;		// ���̎�̌`
	Hand answerMyHand;	// �����̉�

	int displayCount;	// �\������
	bool result;		// ����
	int trueNum;		// ����

	std::vector<std::shared_ptr<Button>> buttons;

public:
	Game1();
	~Game1();

	void Update(const Peripheral& p)override final;
	void Draw()override final;
};

