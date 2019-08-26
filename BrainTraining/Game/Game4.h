#pragma once
#include "../Scene/Scene.h"
#include <vector>
#include <memory>

class Button;

///�Ђ炪�Ȍv�Z
///HiraganaMath
class Game4 :
	public Scene
{
private:
	void (Game4::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	void DescriptionUpdate(const Peripheral& p);		// ���[������
	void QuestionDisplayUpdate(const Peripheral& p);	// ���\��
	void AnswerCheckUpdate(const Peripheral& p);		// �񓚊m�F
	void AnswerDisplayUpdate(const Peripheral& p);		// �񓚕\��

	void (Game4::*drawer)();	// �h���[�̊֐��|�C���^

	void TitleDraw();			// �~�j�Q�[���^�C�g���`��
	void DescriptionDraw();		// ���[�������`��
	void GameDraw();			// �Q�[�����C���`��

	void CreateQuestion();		// ���쐬

	int nowQNum;	// ���݂̖��ԍ�

	int displayCount;	// �\������

	std::vector<std::shared_ptr<Button>> buttons;

public:
	Game4();
	~Game4();

	void Update(const Peripheral& p);
	void Draw();
};

