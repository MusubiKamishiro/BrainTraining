#pragma once
#include "Geometry.h"

class Peripheral;

class Button
{
private:
	Rect rect;
	bool flag;	// �{�^���͈͓̔��ɂ����true�ɂȂ�

public:
	///@param rect �{�^���̍��W, ��, ����
	Button(Rect rect);
	~Button();

	// �}�E�X�̈ʒu�ƃ{�^���͈͓̔��ɂ���Ƃ���
	// ���N���b�N�����true��Ԃ�
	bool Update(const Peripheral& p);

	// �{�^���̕`��
	void Draw();
};

