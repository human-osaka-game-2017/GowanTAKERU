#include"DirectXGraphics.h"
#include"DirectXInput.h"
#include"DirectXSound.h"
#include"CommonRender.h"
#include"CommonControl.h"
#include"SceneManagement.h"
#include"FileManagement.h"
#include"MainRender.h"
#include"TitleRender.h"
#include"GameClearRender.h"
#include"GameOverRender.h"
#include"BulletControl.h"

#include<tchar.h>
#include<stdio.h>
//#include<math.h>


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
	LeadEnemyData();
	//LoadBulletData();


	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	DWORD prevtime = timeGetTime();
	DWORD currenttime;
	int frcnt = 0;

	//timeBeginPeriod(1);

	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != false) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

			currenttime = timeGetTime();

			if (currenttime - prevtime >= 1000 / 60) {
				SceneManage();
				frcnt++;
			}

//#ifdef _DEBUG
			if (currenttime - prevtime >= 1000) {
				OutputDebug_Number((float)frcnt, hWnd);
				frcnt = 0;
				prevtime = currenttime;

			}
//#endif _DEBUG

		}
		//timeEndPeriod(1);
	}

	//リリース
	SCENE_ID scene_ID = GetCurrentScene_ID();
	int num = 0;
	switch (scene_ID) {
	case MAINSCENE:
		num = MAINSCENE_TEXMAX;
		break;
	case TITLESCENE:
		num = TITLESCENE_TEXMAX;
		break;
	case GAMECLEARSCENE:
		num = GAMECLEAR_TEXMAX;
		break;
	case GAMEOVERSCENE:
		num = GAMECLEAR_TEXMAX;
		break;
	}
	ReleaseTexture(num);
	FreeTexture();
	ReleaseBlackOutTexture();

	//switch (scene_ID) {
	//case MAINSCENE:
	//	num = MAINSCENE_SOUNDMAX;
	//	break;
	//case TITLESCENE:
	//	num = TITLESCENE_SOUNDMAX;
	//	break;
	//case GAMECLEARSCENE:
	//	num = GAMECLEAR_SOUNDMAX;
	//	break;
	//case GAMEOVERSCENE:
	//	num = GAMECLEAR_SOUNDMAX;
	//	break;
	//}
	//ReleaseBuffer(num);

	ReleaseSoundDevice();
	FreeDxInput();
	FreeDxGraphics();

	return (int)msg.wParam;
}

void OutputDebug_Number(float outputNum, HWND hWnd) {

	/*int digit = (int)log10((double)outputNum) + 1;
		TCHAR* buff = (TCHAR*)malloc(sizeof(TCHAR)*digit);
	_stprintf_s(buff, digit, _T("%d\n"), outputNum);
	OutputDebugString(buff);*/

	//char *buff2 = (char*)malloc(256);
	//sprintf(buff2, "%d", 110);
	TCHAR buff[256];
	_stprintf_s(buff, 256, _T("%f\n"), outputNum);
	OutputDebugString(buff);
	SetWindowText(hWnd, buff);

}