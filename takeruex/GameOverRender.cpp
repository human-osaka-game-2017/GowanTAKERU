#include"GameOverRender.h"
#include"CommonRender.h"
#include"DirectXGraphics.h"
#include"FileManagement.h"

void GameOverBackGroundRender() {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	CUSTOMVERTEX Backgrond[] = {
		{ 0.0f,0.0f,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ DISPLAY_WIDTH,0.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ DISPLAY_WIDTH,DISPLAY_HEIGHT,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ 0.0f,DISPLAY_HEIGHT,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	// テクスチャをステージに割り当てる
	pD3Device->SetTexture(0, pTexture[GAMEOVERBACKGROUND_TEX]);
	// 描画
	pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Backgrond, sizeof(CUSTOMVERTEX));
}

#define GAMEOVERLOGO_HEIGHT 250
#define GAMEOVERLOGO_WIDTH 800
#define GAMEOVERLOGO_X 640
#define GAMEOVERLOGO_Y 300
#define FADEINFRM 300

void GameOverLogoRender() {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	CUSTOMVERTEX gameOverLogo[] = {
		{ -GAMEOVERLOGO_WIDTH / 2, -GAMEOVERLOGO_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
		{ GAMEOVERLOGO_WIDTH / 2, -GAMEOVERLOGO_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ GAMEOVERLOGO_WIDTH / 2,  GAMEOVERLOGO_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ -GAMEOVERLOGO_WIDTH / 2,  GAMEOVERLOGO_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	static int frcnt = 0;
	DWORD color = 0xFFFFFFFF;

	
	if (frcnt < (FADEINFRM)) {
		color = ((0xFF * frcnt / (FADEINFRM)) << 24) | 0x00FFFFFF;
	}

	CUSTOMVERTEX drawVertex[4];
	for (int i = 0; i < 4; i++) {
		drawVertex[i] = gameOverLogo[i];
		drawVertex[i].x += GAMEOVERLOGO_X;
		drawVertex[i].y += GAMEOVERLOGO_Y;
		drawVertex[i].color = color;
	}

		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[GAMEOVERLOGO_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawVertex, sizeof(CUSTOMVERTEX));

	frcnt++;

	if (frcnt < FADEINFRM) {
		frcnt++;
	}
}

#define GAMEOVERPUSH_Z_KEY_HEIGHT 250
#define GAMEOVERPUSH_Z_KEY_WIDTH 800
#define GAMEOVERPUSH_Z_KEY_X 640
#define GAMEOVERPUSH_Z_KEY_Y 525
#define FADEFRM 180

void GameOverPushZkeyRender(bool pushKey) {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	CUSTOMVERTEX titleLogo[] = {
		{ -GAMEOVERPUSH_Z_KEY_WIDTH / 2, -GAMEOVERPUSH_Z_KEY_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
		{ GAMEOVERPUSH_Z_KEY_WIDTH / 2, -GAMEOVERPUSH_Z_KEY_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ GAMEOVERPUSH_Z_KEY_WIDTH / 2,  GAMEOVERPUSH_Z_KEY_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ -GAMEOVERPUSH_Z_KEY_WIDTH / 2,  GAMEOVERPUSH_Z_KEY_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
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
		drawVertex[i].x += GAMEOVERPUSH_Z_KEY_X;
		drawVertex[i].y += GAMEOVERPUSH_Z_KEY_Y;
		drawVertex[i].color = color;
	}

	if (pushKey) {
		if ((frcnt % 4) < 2) {
			// テクスチャをステージに割り当てる
			pD3Device->SetTexture(0, pTexture[GAMEOVERPUSH_Z_KEY_TEX]);
			// 描画
			pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawVertex, sizeof(CUSTOMVERTEX));
		}
	}
	else {
		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[GAMEOVERPUSH_Z_KEY_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawVertex, sizeof(CUSTOMVERTEX));
	}

	frcnt++;

	if (frcnt == FADEFRM) {
		frcnt = 0;
	}
}
