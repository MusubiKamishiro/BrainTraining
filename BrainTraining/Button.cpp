#include "Button.h"
#include <DxLib.h>
#include "Peripheral.h"


Button::Button(Rect rect, int imgH)
{
	this->rect = rect;
	flag = false;
	img = imgH;
}


Button::~Button()
{
}

bool Button::Update(const Peripheral& p)
{
	// ボタンの中にマウスがあるとボタンを塗りつぶす(仮)
	auto mousePos = p.GetMousePos();
	if ((rect.Left() < mousePos.x) && (rect.Right() > mousePos.x) && (rect.Top() < mousePos.y) && (rect.Bottom() > mousePos.y))
	{
		flag = true;
	}
	else
	{
		flag = false;
	}

	// ボタンが押されるとtrueを返す
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
	if (img != -1)
	{
		DxLib::DrawExtendGraph(rect.Left(), rect.Top(), rect.Right(), rect.Bottom(), img, true);
	}
}

Rect Button::GetButtonRect() const
{
	return rect;
}
