#pragma once

// �}�E�X�̓��͂𐧌�
class Peripheral
{
private:
	int mouseState;		// ���݂̃}�E�X�̓��͏��
	int lastMouseState;	// ���O�ڰт̃}�E�X�̓��͏��

public:
	Peripheral();
	~Peripheral();

	// ���͏��̍X�V(���ڰ�)
	void Update();

	// ���݂̉�����Ԃ̌��o
	// @param mouseid�@���ׂ����}�E�X�̃{�^��
	// @pretval true �����Ă�, false �����ĂȂ�
	bool IsPressing(int mouseid)const;

	// ���݂��ضް���(�������u��)�̌��o
	bool IsTrigger(int mouseid)const;

	void DebugDraw();
};

