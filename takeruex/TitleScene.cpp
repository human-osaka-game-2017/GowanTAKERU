#include"TitleScene.h"
#include"TitleRender.h"
#include"CommonControl.h"
#include"FileManagement.h"
#include"DirectXGraphics.h"
#include"BlackOutRender.h"
#include"DirectXSound.h"

//プロトタイプ群
void TitleRender(bool pushkey);

SCENE_ID RunTitleScene(bool willbetrancefar) {
	SCENE_ID nextscene_ID = TITLESCENE;

	KEYSTATE* key = GetKey();
	KeyCheck(&key[KEY_Z], DIK_Z);

	static bool nextsceneflg = false;
	static int step = 0;


	if (willbetrancefar) {
		step = 2;
	}

	switch (step) {
	case 0:
		TitleSceneLoad();
		nextsceneflg = false;
		PlayBackSound(TITLESCENE_BGM01, true, 10000000);
		step++;
		break;

	case 1:

		if (key[KEY_Z] == KEY_PUSH) {
			nextscene_ID = MAINSCENE;
			nextsceneflg = true;
		}

		TitleRender(nextsceneflg);

		break;

	case 2:
		ReleaseTexture(TITLESCENE_TEXMAX);
		FreeTexture();
		StopSound(TITLESCENE_BGM01);
		step = 0;
		break;
	}

	return nextscene_ID;
}

void TitleRender(bool pushkey) {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();

	pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0xff, 0xff, 0xff),
		1.0, 0);

	pD3Device->BeginScene();

	TitleBackGroundRender();
	TitleLogoRender(pushkey);

	BlackOutData* blackOutData = GetBlackOutData();
	if (blackOutData->BlackOutflg) {
		BlackOutRender();
	}

	pD3Device->EndScene();
	pD3Device->Present(NULL, NULL, NULL, NULL);
}

