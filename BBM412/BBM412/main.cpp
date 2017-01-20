#ifndef _DEBUG
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "Game.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_	LPWSTR    lpCmdLine,
	_In_	int       nCmdShow)
{

	Game * game = new Game();
	return game->run();
}
#else
#include "Game.h"
int main(int argc, char **argv)
{
	try {

	Game * game = new Game();
	return game->run();

	}
	catch (GException e) {
		int i;
		cout << "Main()->" << e.what() << endl;
		cin >> i;
	}
}
#endif