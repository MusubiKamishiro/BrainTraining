#pragma once
#include "../Scene/Scene.h"
#include <memory>
#include <vector>
#include <map>
#include <array>

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

	void DescriptionUpdate(const Peripheral& p);		// ルール説明
	void QuestionDisplayUpdate(const Peripheral& p);	// 問題表示
	void AnswerDisplayUpdate(const Peripheral& p);		// 回答表示

	void (Game1::*drawer)();	// ドローの関数ポインタ

	void TitleDraw();			// ミニゲームタイトル描画
	void DescriptionDraw();		// ルール説明描画
	void GameDraw();			// ゲームメイン描画

	int rock, paper, scissors;	// グーちょきぱーの画像ハンドル

	std::map<int, std::string> questionStatements;	// 問題文
	std::array<int, 3> questionHands;	// 問題の手

	int qNum;
	int handNum;
	int displayCount;

	std::vector<std::shared_ptr<Button>> buttons;

public:
	Game1();
	~Game1();

	void Update(const Peripheral& p);
	void Draw();
};

