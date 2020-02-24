#pragma once

#include <vector>
#include <array>
#include "../Geometry.h"
#include "../Scene/Scene.h"

class Button;

/// 色のID
enum class COLOR
{
	RED,
	BLUE,
	YELLOW,
	GREEN,
	MAX
};

/// 色の最大数
#define COLOR_SIZE (static_cast<int>(COLOR::MAX))

class Game6 :
	public Scene
{
private:
	void (Game6::*_updater)(const Peripheral& p);
	void (Game6::*_drawer)();

	/// 関数ポインターを使用して、ゲームの状態を管理するもの
	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);

	void StartUpdate(const Peripheral& p);
	void ExpUpdate(const Peripheral& p);
	void CntDownUpdate(const Peripheral& p);
	void GameUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	void ChangeButton();						// ボタンの位置変更
	void ChangeColor();							// 文の変更
	void ButtonUpdater(const Peripheral& p);	// ボタンが入力されたかの更新用
	
	void StartDraw();							// ゲーム開始前の描画
	void ExpDraw();								// 説明テキストの描画
	void CntDownDraw();							// カウントダウン時の描画
	void GameDraw();							// ゲーム中の描画

	std::vector<COLOR> _btnType;									// ボタンの色情報
	std::array<std::shared_ptr<Button>, COLOR_SIZE> _buttons;		// ゲーム中のボタン
	std::array<std::string, COLOR_SIZE> _texts;						// 色のID 
	std::array<int, COLOR_SIZE> _colors;							// ゲーム説明用の画像
	std::array<int, 3> _expImgs;

	Vector2 _strSize;		// 文字の大きさ		

	int _questions;
	int _corrects;			// 正解数
	int _timeCnt;
	int _expCnt;
	int _blindCnt;
	int _correctImg, _missImg;
	int _textNum, _colorNum;		// お題の文字と色の取得用
	int _correctSE, _missSE, _cntDownSE, _startSE;
	int _gameBGM;

	bool _isColor;			// 色を変更したかの判定用

	const Size _btnSize;

public:
	Game6();
	~Game6();

	void Update(const Peripheral& p);
	void Draw();

};