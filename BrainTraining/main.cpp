#include "Game.h"
#include <DxLib.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Game& game = Game::Instance();
	game.Initialize();
	game.Run();
	game.Terminate();
}
