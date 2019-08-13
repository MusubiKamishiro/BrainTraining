#pragma once
#include "../Scene/Scene.h"
#include <memory>
#include <vector>

class Button;

///後出しじゃんけん
///LaterRPS
class Game1 :
	public Scene
{
private:
	void (Game1::*updater)(const Peripheral& p);	// アップデートの関数ポインタ

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	void DescriptionUpdate(const Peripheral& p);	// ルール説明
	void GameUpdate(const Peripheral& p);			// ゲームメイン

	void (Game1::*drawer)();	// ドローの関数ポインタ

	void TitleDraw();			// ミニゲームタイトル描画
	void DescriptionDraw();		// ルール説明描画
	void GameDraw();			// ゲームメイン描画

	int rock, paper, scissors;	// グーちょきぱーの画像ハンドル

	int count = 0;
	int n = 0;

	std::vector<std::shared_ptr<Button>> buttons;

public:
	Game1();
	~Game1();

	void Update(const Peripheral& p);
	void Draw();
};

