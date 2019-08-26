#pragma once
#include "../Scene/Scene.h"
#include <vector>
#include <map>
#include <memory>

class Button;


// 演算子
enum class Operator
{
	PLUS,
	MINUS,
	MULTI,
	MAX
};

///ひらがな計算
///HiraganaMath
class Game4 :
	public Scene
{
private:
	void (Game4::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	void DescriptionUpdate(const Peripheral& p);		// ルール説明
	void QuestionDisplayUpdate(const Peripheral& p);	// 問題表示
	void AnswerCheckUpdate(const Peripheral& p);		// 回答確認
	void AnswerDisplayUpdate(const Peripheral& p);		// 回答表示

	void (Game4::*drawer)();	// ドローの関数ポインタ

	void TitleDraw();			// ミニゲームタイトル描画
	void DescriptionDraw();		// ルール説明描画
	void GameDraw();			// ゲームメイン描画

	int RandomNum(int parameter);		// 引数を母数にした乱数の結果を返す
	std::string CreateHiraganaNum(int num);	// 引数の数字をひらがなにする

	void CreateQuestion();		// 問題作成
	void SelectNum(int num);
	void SelectOperator();

	std::map<int, std::string> questionOperators;
	std::map<int, std::string> hiraganaNum;

	int nowQNum;	// 現在の問題番号
	std::string question;	// 問題文

	int op;		// 問題演算記号

	int displayCount;	// 表示時間

	std::vector<std::shared_ptr<Button>> buttons;

public:
	Game4();
	~Game4();

	void Update(const Peripheral& p);
	void Draw();
};

