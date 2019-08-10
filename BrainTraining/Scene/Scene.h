#pragma once
#include <memory>

class Peripheral;

// (純粋仮想ｸﾗｽ)
class Scene
{
protected:
	// 描画ブレンドモードのパラメータ(0~255)
	int pal;

public:
	Scene();
	virtual ~Scene();

	// ｼｰﾝの更新を行う
	virtual void Update(const Peripheral& p) = 0;
	// シーンの描画
	virtual void Draw() = 0;
	
	// フェードインの処理を書く
	virtual void FadeinUpdate(const Peripheral& p) = 0;
	// フェードアウトの処理を書く
	virtual void FadeoutUpdate(const Peripheral& p) = 0;
	// 待機中の処理を書く
	virtual void WaitUpdate(const Peripheral& p) = 0;

	int GetPal()const;
};

