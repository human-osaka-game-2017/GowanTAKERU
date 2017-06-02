#include"Render.h"
#include"Control.h"
#include"Player.h"

#define TITLE 	TEXT("Basic of game")

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wp, lp);

}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX Wndclass;
	Wndclass.cbSize = sizeof(WNDCLASSEX);
	Wndclass.style = CS_HREDRAW | CS_VREDRAW;
	Wndclass.lpfnWndProc = WndProc;
	Wndclass.cbClsExtra = 0;
	Wndclass.cbWndExtra = 0;
	Wndclass.hInstance = hInstance;
	Wndclass.hIcon = NULL;
	Wndclass.hCursor = NULL;
	Wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	Wndclass.lpszMenuName = NULL;
	Wndclass.lpszClassName = TITLE;
	Wndclass.hIconSm = NULL;

	//Windowの登録
	RegisterClassEx(&Wndclass);

	//Windowの生成
	HWND hWnd = CreateWindow(
		TITLE,								//ウィンドウのクラス名
		"TITLE", 							//ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	//ウィンドウスタイル
		CW_USEDEFAULT,						// ウィンドウの横方向の位置x
		CW_USEDEFAULT,						// ウィンドウの縦方向の位置y
		DISPLAY_WIDTH,							// Width（幅）
		DISPLAY_HEIGHT,							// Height（高さ）
		NULL,
		NULL,
		hInstance,							// アプリケーションインスタンスのハンドル
		NULL
	);
	SetProcessDPIAware();					//お守り

	/*-------------------------------------------
	resize
	-------------------------------------------*/
	/*RECT RectWnd;
	RECT RectCli;
	int cx = 0;
	int cy = 0;
	GetWindowRect(hWnd, &RectWnd);
	GetClientRect(hWnd, &RectCli);
	cx = RectWnd.right - RectCli.right;
	cy = RectWnd.bottom - RectCli.bottom;
	SetWindowPos(hWnd, NULL, NULL, NULL, RectCli.right + cx, RectCli.bottom + cy, SWP_NOMOVE);*/

	//direct graphics初期化
	bool fullscreen = true;
	DXGraphicsInit(hWnd, fullscreen);
	TextureInit();

	//direct input初期化
	DXInputKeybourdInit(hWnd, hInstance);
	
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	DWORD oldtime = timeGetTime();
	DWORD currenttime;

	PlayerInit();

	timeBeginPeriod(1);

	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != false) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			currenttime = timeGetTime();
			if (currenttime - oldtime >= 1000 / 60) {
				oldtime = currenttime;

				/*Contorol();*/
				Render();

			}
			else {
				Sleep(1);
			}
		}
		timeEndPeriod(1);
	}

	FreeDxInput();
	FreeDxGraphics();

	return (int)msg.wParam;
}