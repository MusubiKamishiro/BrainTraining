#pragma once
#include "Geometry.h"

class Peripheral;

class Button
{
private:
	Rect rect;
	bool flag;	// ボタンの範囲内にいるとtrueになる
	int img;	// 画像ハンドル

public:
	///@param rect ボタンの座標, 幅, 高さ
	///@param imgH ボタンにつけたい画像ハンドル(描画の際にボタンの大きさに拡縮されるので注意)
	Button(Rect rect, int imgH = -1);
	~Button();

	// マウスの位置とボタンの範囲内にあるときに
	// 左クリックするとtrueを返す
	bool Update(const Peripheral& p);

	// ボタンの描画
	void Draw();

	// ボタンの矩形を返す
	Rect GetButtonRect()const;
};

