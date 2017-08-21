#include"GameClearRender.h"
#include"DirectXGraphics.h"
#include"CommonRender.h"
#include"FileManagement.h"

void GameCleaRollRender(GameClearObj* pGameClear) {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	CUSTOMVERTEX gameClear[] = {
		{ 0.0f, -GAMECLEARHEIGTH, 0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
		{ GAMECLEARWIDTH, -GAMECLEARHEIGTH, 0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ GAMECLEARWIDTH,  0.0f, 0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ 0.0f,  0.0f, 0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	CUSTOMVERTEX drawVertex[4];
	for (int i = 0; i < 4; i++) {
		drawVertex[i] = gameClear[i];
		drawVertex[i].x += pGameClear->Pos.x;
		drawVertex[i].y += pGameClear->Pos.y;
	}

	// テクスチャをステージに割り当てる
	pD3Device->SetTexture(0, pTexture[GAMECLEARROLL_TEX]);
	// 描画
	pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawVertex, sizeof(CUSTOMVERTEX));
}


#define GAMECLEARPUSH_Z_KEY_HEIGHT 250
#define GAMECLEARPUSH_Z_KEY_WIDTH 800
#define GAMECLEARPUSH_Z_KEY_X 640
#define GAMECLEARPUSH_Z_KEY_Y 525
#define FADEFRM 180

void GameClearPushZkeyRender(bool pushKey) {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	CUSTOMVERTEX titleLogo[] = {
		{ -GAMECLEARPUSH_Z_KEY_WIDTH / 2, -GAMECLEARPUSH_Z_KEY_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
		{ GAMECLEARPUSH_Z_KEY_WIDTH / 2, -GAMECLEARPUSH_Z_KEY_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ GAMECLEARPUSH_Z_KEY_WIDTH / 2,  GAMECLEARPUSH_Z_KEY_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ -GAMECLEARPUSH_Z_KEY_WIDTH / 2,  GAMECLEARPUSH_Z_KEY_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	static int frcnt = 0;
	DWORD color = 0xFFFFFFFF;

	if (!pushKey) {
		if (frcnt < (FADEFRM / 2)) {
			color = ((0xFF * frcnt / (FADEFRM / 2)) << 24) | 0x00FFFFFF;
		}
		else if (frcnt >(FADEFRM / 2)) {
			color = ((0xFF * (FADEFRM - frcnt) / (FADEFRM / 2)) << 24) | 0x00FFFFFF;
		}
	}

	CUSTOMVERTEX drawVertex[4];
	for (int i = 0; i < 4; i++) {
		drawVertex[i] = titleLogo[i];
		drawVertex[i].x += GAMECLEARPUSH_Z_KEY_X;
		drawVertex[i].y += GAMECLEARPUSH_Z_KEY_Y;
		drawVertex[i].color = color;
	}

	if (pushKey) {
		if ((frcnt % 4) < 2) {
			// テクスチャをステージに割り当てる
			pD3Device->SetTexture(0, pTexture[GAMECLEARPUSH_Z_KEY_TEX]);
			// 描画
			pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawVertex, sizeof(CUSTOMVERTEX));
		}
	}
	else {
		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[GAMECLEARPUSH_Z_KEY_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawVertex, sizeof(CUSTOMVERTEX));
	}

	frcnt++;

	if (frcnt == FADEFRM) {
		frcnt = 0;
	}
}
