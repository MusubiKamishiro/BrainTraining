#pragma once
#include "../Scene/Scene.h"
#include <vector>
#include <map>
#include <memory>

class Button;


// ���Z�q
enum class Operator
{
	PLUS,
	MINUS,
	MULTI,
	MAX
};

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

	void EditMyAnswer(unsigned int num);

	int RandomNum(int parameter);			// ������ꐔ�ɂ��������̌��ʂ�Ԃ�
	std::string CreateHiraganaNum(int num);	// �����̐������Ђ炪�Ȃɂ���

	void CreateQuestion();		// ���쐬
	void SelectNum(int num, int& qnum);	// num...���̍ő吔
	void SelectOperator();

	int buttonImg;
	int trueSE, falseSE;		// ����,�s�����̎��̌��ʉ�

	std::map<int, std::string> questionOperators;
	std::map<int, std::string> hiraganaNum;

	int nowQNum;	// ���݂̖��ԍ�
	std::string question;	// ��蕶

	int firstNum, secondNum;	// ��萔�l
	int qAnswer, myAnswer;		// ���̉�, �����̉�
	int trueNum;	// ����
	bool result;

	int op;		// ��艉�Z�L��

	int displayCount;	// �\������

	std::vector<std::shared_ptr<Button>> buttons;
	std::shared_ptr<Button> decide, del;	// ����,�폜

public:
	Game4();
	~Game4();

	void Update(const Peripheral& p);
	void Draw();
};

