#pragma once
#include "Geometry.h"


// マウスの入力を制御
class Peripheral
{
private:
	int mouseState;		// 現在のマウスの入力情報
	int lastMouseState;	// 直前ﾌﾚｰﾑのマウスの入力状態

	Vector2 mousePos;	// マウスの座標

public:
	Peripheral();
	~Peripheral();

	// 入力情報の更新(毎ﾌﾚｰﾑ)
	void Update();

	// 現在の押下状態の検出
	// @param mouseid　調べたいマウスのボタン
	// @pretval true 押してる, false 押してない
	bool IsPressing(int mouseid)const;

	// 現在のﾄﾘｶﾞｰ状態(押した瞬間)の検出
	bool IsTrigger(int mouseid)const;

	Vector2 GetMousePos()const;

	// マウスの情報を表示
	// デバック中のみ使用すること
	void DebugDraw();
};

