#include"TitleRender.h"
#include"CommonRender.h"
#include"DirectXGraphics.h"
#include"FileManagement.h"

void TitleBackGroundRender() {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	CUSTOMVERTEX Backgrond[] = {
		{ 0.0f,0.0f,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ DISPLAY_WIDTH,0.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ DISPLAY_WIDTH,DISPLAY_HEIGHT,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ 0.0f,DISPLAY_HEIGHT,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};
	
		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[TITLE_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Backgrond, sizeof(CUSTOMVERTEX));
}

#define TITLELOGO_HEIGHT 250
#define TITLELOGO_WIDTH 800
#define TITLELOGO_X 670
#define TITLELOGO_Y 525
#define FADEFRM 180

void TitleLogoRender(bool pushKey) {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	CUSTOMVERTEX titleLogo[] = {
		{ -TITLELOGO_WIDTH / 2, -TITLELOGO_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
		{ TITLELOGO_WIDTH / 2, -TITLELOGO_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ TITLELOGO_WIDTH / 2,  TITLELOGO_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ -TITLELOGO_WIDTH / 2,  TITLELOGO_HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
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
		drawVertex[i].x += TITLELOGO_X;
		drawVertex[i].y += TITLELOGO_Y;
		drawVertex[i].color = color;
	}

	if (pushKey) {
		if ((frcnt % 4) < 2) {
			// テクスチャをステージに割り当てる
			pD3Device->SetTexture(0, pTexture[TITLELOGO_TEX]);
			// 描画
			pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawVertex, sizeof(CUSTOMVERTEX));
		}
	}
	else {
		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[TITLELOGO_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawVertex, sizeof(CUSTOMVERTEX));
	}
	
	frcnt++;

	if (frcnt == FADEFRM) {
		frcnt = 0;
	}
}
