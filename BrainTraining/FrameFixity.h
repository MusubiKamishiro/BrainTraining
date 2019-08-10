#pragma once
#include <windows.h>

// フレームレート固定用クラス
// windowsプログラムなのでwinmainを記述しないといけない
class FrameFixity
{
private:
	HWND hWnd;		// ウィンドウハンドル用変数
	MSG msg;		// メッセージを受け取る変数
	HDC hdc;		// コンテキストデバイスのハンドル

	int fps = 60;		// フレームレート
	long long end = 0;	// フレームレート調整用変数
	long long next = 0;	// フレームレート調整用変数

	FrameFixity();
	FrameFixity(const FrameFixity&) {};
	void operator=(const FrameFixity&) {};

	long long GetCTime();
public :
	// インスタンスを取得する関数
	static FrameFixity& Instance() {
		static FrameFixity instance;
		return instance;
	}
	~FrameFixity();

	// 各変数などの初期化処理
	// 最初に一回だけ呼ぶ
	void FFInitialize();

	// メッセージを受け取ったかどうかをチェックする
	// メインループ内で毎回呼ぶ
	bool CheckReceiveMessage();

	// この関数で帰ってきたメッセージがWM_QUITだったら終了
	// CheckReceiveMessage()でtrueが帰ってきた場合に使用
	MSG GetReceiveMessage();

	// CheckReceiveMessage()がfalseだった時に使用
	// この関数でフレームの調整を行う
	void AdjustmentFrameLate();

	// 後処理を行う
	// 具体的には使用したデバイスコンテキストを解放している
	void Terminate();
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////記述例/////////////////////////////////////////
/*
#include <windows.h>
#include <DxLib.h>
#include "FrameFixity.h"

using namespace DxLib;

int WINAPI WinMain(HISNTACE hInstance, HISNTACE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	FrameFixity& ff = FrameFixity::Instance();
	// 初期化処理
	// ～他の初期化処理～////
	ff.FFInitialize();
	/////////////////////////

	while (ProcessMessage() == 0) {
		if (ff.CheckReceiveMessage()) {
			if (ff.GetReceiveMessage() == WM_QUIT) {
				break;
			}
		}
		else {
			///////// ～メイン処理～/////////
			ff.AdjustmentFrameLate();
			/////////////////////////////////
		}
	}

	// 後処理
	////////// ～他の後処理～////////////
	ff.Terminate();
	////////////////////////////////////
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////