#include "Peripheral.h"
#include <DxLib.h>


Peripheral::Peripheral()
{
}


Peripheral::~Peripheral()
{
}

void Peripheral::Update()
{
	lastMouseState = mouseState;
	mouseState = DxLib::GetMouseInput();

	DxLib::GetMousePoint(&mousePos.x, &mousePos.y);
}

bool Peripheral::IsPressing(int mouseid) const
{
	return (mouseState & mouseid);
}

bool Peripheral::IsTrigger(int mouseid) const
{
	return (!(lastMouseState & mouseid) && (mouseState & mouseid));
}

Vector2 Peripheral::GetMousePos() const
{
	return mousePos;
}

void Peripheral::DebugDraw()
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	SetFontSize(24);
	DxLib::DrawFormatString(0, 0, 0xffffff, "マウスの座標(%d, %d)", mousePos.x, mousePos.y);
}
