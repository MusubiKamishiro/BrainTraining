#include "FrameFixity.h"
#include <functional>
#include <chrono>
#include <thread>


FrameFixity::FrameFixity()
{
}

FrameFixity::~FrameFixity()
{
}

void FrameFixity::FFInitialize()
{
	// フレームレート調整系変数初期化
	hWnd = {};
	msg = {};
	hdc = GetDC(hWnd);
	end = GetCTime();
	next = end + (1000 * 1000 / fps);
}

bool FrameFixity::CheckReceiveMessage()
{
	// 投げられたメッセージを変数msgに格納
	// メッセージがあった場合はtrueが帰ってくる
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// 取得したメッセージを文字列メッセージに変換
		// 変換したメッセージをウィンドウプロシージャに投げる
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
		return true;
	}
	else
	{
		// メッセ―ジを受け取っていないときにはfalseを返す
		return false;
	}
}

MSG FrameFixity::GetReceiveMessage()
{
	return msg;
}

void FrameFixity::AdjustmentFrameLate()
{
		// フレームレート調整系処理
	std::this_thread::sleep_for(std::chrono::milliseconds(5));	// 指定した相対時間だけスレッドをブロックする
	end = GetCTime();
	if (end < next)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(next - end));
		next += (1000 * 1000 / fps);
	}
	else
	{
		next = end + (1000 * 1000 / fps);
	}
}

void FrameFixity::Terminate()
{
	// デバイスコンテキスト解放
	ReleaseDC(hWnd, hdc);
}

// ウィンドウから投げられてくるメッセージをキャッチして処理する
// キー、マウス、描画などの様々なメッセージがここに投げられる
// messageにキャッチしたメッセージが入っている
// WM_DESTROYはウィンドウを画面から消すときに呼ばれる
//LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message)
//	{
//	case WM_DESTROY:
//		// メインウィンドウを閉じられた時や、アプリケーションを終了するときに
//		// この関数を呼んで終了処理をする
//		PostQuitMessage(0);
//		break;
//	default:
//		// システムに処理を任せる時はこの関数を返す
//		// 自分で処理をしたときは0を返す
//		return DefWindowProc(hwnd, message, wParam, lParam);
//	}
//	return 0L;
//}

long long FrameFixity::GetCTime()
{
	// 現在の時間をSystemClockから取得し、microsecondsに変換してその値を返す
	std::function<long long(void)> currentTimeMicro = []()
	{
		std::chrono::system_clock::duration d = std::chrono::system_clock::now().time_since_epoch();
		return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
	};
	return currentTimeMicro();
}
