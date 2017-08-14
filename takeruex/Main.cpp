#include"DirectXGraphics.h"
#include"DirectXInput.h"
#include"DirectXSound.h"
#include"CommonRender.h"
#include"SceneManagement.h"
#include"FileManagement.h"
#include"MainRender.h"
#include"TitleRender.h"


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
	PSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;

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
	Wndclass.lpszClassName = TEXT("TITLE");
	Wndclass.hIconSm = NULL;

	//Windowの登録
	RegisterClassEx(&Wndclass);

	//Windowの生成
	hWnd = CreateWindow(
		TEXT("TITLE"),								//ウィンドウのクラス名
		"TAKERU", 							//ウィンドウのタイトル
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

											
	DXGraphicsInit(hWnd, true);				//初期化処理
	TextureInit();
	DXInputKeybourdInit(hWnd, hInstance);
	DirectXSoundInit(hWnd);
	StageMapNumMaxInit();


	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	DWORD oldtime = timeGetTime();
	DWORD currenttime;

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

				SceneManage();

			}
			else {
				Sleep(1);
			}
		}
		timeEndPeriod(1);
	}

	//リリース
	SCENE_ID scene_ID = GetCurrentScene_ID();
	int num = 0;
	switch (scene_ID) {
	case MAINSCENE:
		num = MAINSCENE_TEXMAX;
	case TITLESCENE:
		num = TITLESCENE_TEXMAX;
	}
	ReleaseTexture(num);
	FreeTexture();
	ReleaseBuffer();
	ReleaseSoundDevice();
	ReleaseBlackOutTexture();
	FreeDxInput();
	FreeDxGraphics();

	return (int)msg.wParam;
}
