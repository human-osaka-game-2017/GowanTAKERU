#include"GameOver.h"
#include"GameOverRender.h"
#include"CommonControl.h"
#include"FileManagement.h"
#include"DirectXGraphics.h"
#include"BlackOutRender.h"
#include"DirectXSound.h"

//プロトタイプ群
void GameOverRender(bool pushkey);

SCENE_ID RunGameOverScene(bool willbetrancefar) {
	SCENE_ID nextscene_ID = GAMEOVERSCENE;

	KEYSTATE* key = GetKey();
	KeyCheck(&key[KEY_Z], DIK_Z);

	static bool nextsceneflg = false;
	static int step = 0;


	if (willbetrancefar) {
		step = 2;
	}

	switch (step) {
	case 0:
		GameOverSceneLoad();
		PlayBackSound(GAMEOVERSCENE_BGM01, false, 100);
		nextsceneflg = false;
		step++;
		break;

	case 1:

		if (key[KEY_Z] == KEY_PUSH) {
			nextscene_ID = TITLESCENE;
			nextsceneflg = true;
		}

		GameOverRender(nextsceneflg);

		break;

	case 2:
		ReleaseTexture(GAMEOVER_TEXMAX);
		FreeTexture();
		step = 0;
		break;
	}

	return nextscene_ID;
}

void GameOverRender(bool pushkey) {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();

	pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0xff, 0xff, 0xff),
		1.0, 0);

	pD3Device->BeginScene();

	GameOverBackGroundRender();
	GameOverLogoRender();
	GameOverPushZkeyRender(pushkey);

	BlackOutData* blackOutData = GetBlackOutData();
	if (blackOutData->BlackOutflg) {
		BlackOutRender();
	}

	pD3Device->EndScene();
	pD3Device->Present(NULL, NULL, NULL, NULL);
}


