#pragma once
#include "../Scene/Scene.h"
#include <memory>
#include <vector>
#include <map>
#include <array>

class Button;

// 手の形
enum class Hand
{
	ROCK,		// グー
	SCISSORS,	// チョキ
	PAPER,		// パー
	MAX
};

// 問題
enum class Question
{
	WIN,		// 勝ってください
	LOSE,		// 負けてください
	DONOTWIN,	// 勝たないでください
	DONOTLOSE,	// 負けないでください
	MAX
};

// じゃんけんの結果
enum class Result
{
	WIN,
	DRAW,
	LOSE,
	MAX
};

///後出しじゃんけん
///LaterRPS
class Game1 :
	public Scene
{
private:
	void (Game1::*updater)(const Peripheral& p);	// アップデートの関数ポインタ

	void FadeinUpdate(const Peripheral& p)override final;
	void FadeoutUpdate(const Peripheral& p)override final;
	void WaitUpdate(const Peripheral& p)override final;

	void DescriptionUpdate(const Peripheral& p);		// ルール説明
	void QuestionDisplayUpdate(const Peripheral& p);	// 問題表示
	void AnswerCheckUpdate(const Peripheral& p);		// 回答確認
	void AnswerDisplayUpdate(const Peripheral& p);		// 回答表示

	void (Game1::*drawer)();	// ドローの関数ポインタ

	void TitleDraw();			// ミニゲームタイトル描画
	void DescriptionDraw();		// ルール説明描画
	void GameDraw();			// ゲームメイン描画

	void CreateQuestion();		// 問題作成
	Result JudgeResult(int& qNum, Hand& myHand);	// じゃんけん結果の確認

	std::vector<int> handImg;	// グーちょきぱーの画像ハンドル
	int rock, paper, scissors;	// ボタンの画像ハンドル
	int trueSE, falseSE;		// 正解,不正解の時の効果音

	std::map<int, std::string> questionStatements;	// 問題文
	std::array<int, static_cast<int>(Hand::MAX)> questionHands;	// 問題の手
	
	int nowQNum;	// 現在の問題番号

	int qStatementNum;	// 問題文章
	int qHandNum;		// 問題の手の形
	Hand answerMyHand;	// 自分の回答

	int displayCount;	// 表示時間
	bool result;		// 結果
	int trueNum;		// 正解数

	std::vector<std::shared_ptr<Button>> buttons;

public:
	Game1();
	~Game1();

	void Update(const Peripheral& p)override final;
	void Draw()override final;
};

