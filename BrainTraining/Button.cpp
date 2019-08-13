#include "Button.h"
#include <DxLib.h>
#include "Peripheral.h"


Button::Button(Rect rect)
{
	this->rect = rect;
	flag = false;
}


Button::~Button()
{
}

bool Button::Update(const Peripheral& p)
{
	// �{�^���̒��Ƀ}�E�X������ƃ{�^����h��Ԃ�(��)
	auto mousePos = p.GetMousePos();
	if ((rect.Left() < mousePos.x) && (rect.Right() > mousePos.x) && (rect.Top() < mousePos.y) && (rect.Bottom() > mousePos.y))
	{
		flag = true;
	}
	else
	{
		flag = false;
	}

	// �{�^������������true��Ԃ�
	if (flag)
	{
		if (p.IsTrigger(MOUSE_INPUT_LEFT))
		{
			return true;
		}
	}

	return false;
}

void Button::Draw()
{
	DxLib::DrawBox(rect.Left(), rect.Top(), rect.Right(), rect.Bottom(), 0x0000ff, flag);
}
