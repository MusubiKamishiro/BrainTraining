#pragma once
#include "Geometry.h"

class Peripheral;

class Button
{
private:
	Rect rect;
	bool flag;	// �{�^���͈͓̔��ɂ����true�ɂȂ�
	int img;	// �摜�n���h��

public:
	///@param rect �{�^���̍��W, ��, ����
	///@param imgH �{�^���ɂ������摜�n���h��(�`��̍ۂɃ{�^���̑傫���Ɋg�k�����̂Œ���)
	Button(Rect rect, int imgH = -1);
	~Button();

	// �}�E�X�̈ʒu�ƃ{�^���͈͓̔��ɂ���Ƃ���
	// ���N���b�N�����true��Ԃ�
	bool Update(const Peripheral& p);

	// �{�^���̕`��
	void Draw();

	// �{�^���̋�`��Ԃ�
	Rect GetButtonRect()const;
};

