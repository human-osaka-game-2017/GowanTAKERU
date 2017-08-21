#include"GameClear.h"
#include"CommonControl.h"
#include"FileManagement.h"
#include"CommonRender.h"
#include"GameClearRender.h"
#include"DirectXGraphics.h"
#include"BlackOutRender.h"

void InitGameClear(GameClearObj* gameclear);
void GameClearRender(bool pushkey, GameClearObj* pGameclear);

SCENE_ID RunGameClearScene(bool willbetrancefar) {

	SCENE_ID nextscene_ID = GAMECLEARSCENE;
	
	static GameClearObj gameClear;

	KEYSTATE* key = GetKey();
	KeyCheck(&key[KEY_Z], DIK_Z);

	static bool nextsceneflg = false;
	static int step = 0;

	if (willbetrancefar) {
		step = 2;
	}

	switch (step) {
	case 0:
		GameClearSceneLoad();
		nextsceneflg = false;
		InitGameClear(&gameClear);
		step++;
		break;

	case 1:

		if (key[KEY_Z] == KEY_PUSH) {
			nextscene_ID = TITLESCENE;
			nextsceneflg = true;
		}

		if (gameClear.Pos.y > DISPLAY_HEIGHT) {
			gameClear.Pos.y += -ROLLSPEED;
		}

		GameClearRender(nextsceneflg,&gameClear);

		break;

	case 2:
		ReleaseTexture(GAMECLEAR_TEXMAX);
		FreeTexture();
		step = 0;
		break;
	}

	return nextscene_ID;
}

void InitGameClear(GameClearObj* pGameclear) {
	pGameclear->Pos.x = 0;
	pGameclear->Pos.y = GAMECLEARHEIGTH;
	pGameclear->frcnt = 0;
}

void GameClearRender(bool pushkey, GameClearObj* pGameclear) {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();

	pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0xff, 0xff, 0xff),
		1.0, 0);

	pD3Device->BeginScene();

	GameCleaRollRender(pGameclear);
	GameClearPushZkeyRender(pushkey);

	BlackOutData* blackOutData = GetBlackOutData();
	if (blackOutData->BlackOutflg) {
		BlackOutRender();
	}

	pD3Device->EndScene();
	pD3Device->Present(NULL, NULL, NULL, NULL);
}
