#pragma once
#include "Geometry.h"

class Peripheral;

class Button
{
private:
	Rect rect;
	bool flag;	// ボタンの範囲内にいるとtrueになる

public:
	///@param rect ボタンの座標, 幅, 高さ
	Button(Rect rect);
	~Button();

	// マウスの位置とボタンの範囲内にあるときに
	// 左クリックするとtrueを返す
	bool Update(const Peripheral& p);

	// ボタンの描画
	void Draw();
};

