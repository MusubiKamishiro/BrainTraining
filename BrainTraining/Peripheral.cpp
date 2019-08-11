#include "Peripheral.h"
#include <DxLib.h>
#include "Geometry.h"


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
}

bool Peripheral::IsPressing(int mouseid) const
{
	return (mouseState & mouseid);
}

bool Peripheral::IsTrigger(int mouseid) const
{
	return (!(lastMouseState & mouseid) && (mouseState & mouseid));
}

void Peripheral::DebugDraw()
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	Vector2 mousePos;
	DxLib::GetMousePoint(&mousePos.x, &mousePos.y);
	DxLib::DrawFormatString(0, 0, 0xffffff, "マウスの座標(%d, %d)", mousePos.x, mousePos.y);

}


