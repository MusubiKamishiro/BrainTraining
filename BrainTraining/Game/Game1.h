#pragma once
#include "../Scene/Scene.h"
#include <memory>
#include <vector>
#include <map>
#include <array>

class Button;

///��o������񂯂�
///LaterRPS
class Game1 :
	public Scene
{
private:
	void (Game1::*updater)(const Peripheral& p);	// �A�b�v�f�[�g�̊֐��|�C���^

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	void DescriptionUpdate(const Peripheral& p);		// ���[������
	void QuestionDisplayUpdate(const Peripheral& p);	// ���\��
	void AnswerDisplayUpdate(const Peripheral& p);		// �񓚕\��

	void (Game1::*drawer)();	// �h���[�̊֐��|�C���^

	void TitleDraw();			// �~�j�Q�[���^�C�g���`��
	void DescriptionDraw();		// ���[�������`��
	void GameDraw();			// �Q�[�����C���`��

	int rock, paper, scissors;	// �O�[���傫�ρ[�̉摜�n���h��

	std::map<int, std::string> questionStatements;	// ��蕶
	std::array<int, 3> questionHands;	// ���̎�

	int qNum;
	int handNum;
	int displayCount;

	std::vector<std::shared_ptr<Button>> buttons;

public:
	Game1();
	~Game1();

	void Update(const Peripheral& p);
	void Draw();
};

