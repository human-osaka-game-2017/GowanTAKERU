#include"MainBlackOutRender.h"
#include"CommonRender.h"
#include"DirectXGraphics.h"
#include"FileManagement.h"
#include"MainRender.h"

bool g_isBlackOut = false;

bool* GetMainBlackOutflg() {
	return &g_isBlackOut;
}

void MainBlackOutRender() {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	CUSTOMVERTEX BlackOut[] = {
		{ 0.0f,0.0f,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ DISPLAY_WIDTH,0.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ DISPLAY_WIDTH,DISPLAY_HEIGHT,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ 0.0f,DISPLAY_HEIGHT,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};
	
	static int frcnt = 0;
	DWORD color;

	if (frcnt < (FINISHFRM / 2)) {
		DWORD aaa = ((0xFF * frcnt / (FINISHFRM / 2)) << 24);
		color = ((0xFF * frcnt / (FINISHFRM / 2)) << 24) | 0x00FFFFFF;
	}
	else {
		color = ((0xFF * (FINISHFRM-frcnt) / (FINISHFRM / 2)) << 24) | 0x00FFFFFF;
	}

	CUSTOMVERTEX DrawVertex[4];
	for (int i = 0; i < 4; i++) {
		DrawVertex[i] = BlackOut[i];
		DrawVertex[i].color = color;
	}

	// テクスチャをステージに割り当てる
	pD3Device->SetTexture(0, pTexture[BLACKOUT_TEX]);
	// 描画
	pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, DrawVertex, sizeof(CUSTOMVERTEX));

	frcnt++;
	if (frcnt == FINISHFRM) {
		g_isBlackOut = false;
		frcnt = 0;
	}
}