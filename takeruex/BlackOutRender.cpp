#include"BlackOutRender.h"
#include"CommonRender.h"
#include"DirectXGraphics.h"
#include"FileManagement.h"
#include"MainRender.h"

BlackOutData g_blackOutData = {
	NON,
	false
};

BlackOutData* GetBlackOutData() {
	return &g_blackOutData;
}

void BlackOutRender() {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetBlackOutTexture();

	CUSTOMVERTEX BlackOut[] = {
		{ 0.0f,0.0f,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ DISPLAY_WIDTH,0.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ DISPLAY_WIDTH,DISPLAY_HEIGHT,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ 0.0f,DISPLAY_HEIGHT,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};
	
	static int frcnt = 0;
	DWORD color;

	if (frcnt < (FINISHFRM / 2)) {
		color = ((0xFF * frcnt / (FINISHFRM / 2)) << 24) | 0x00FFFFFF;
	}
	else if(frcnt > (FINISHFRM / 2)) {
		color = ((0xFF * (FINISHFRM-frcnt) / (FINISHFRM / 2)) << 24) | 0x00FFFFFF;
	}

	CUSTOMVERTEX DrawVertex[4];
	for (int i = 0; i < 4; i++) {
		DrawVertex[i] = BlackOut[i];
		DrawVertex[i].color = color;
	}

	// テクスチャをステージに割り当てる
	pD3Device->SetTexture(0, *pTexture);
	// 描画
	pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, DrawVertex, sizeof(CUSTOMVERTEX));

	if (frcnt == FINISHFRM) {
		g_blackOutData.BlackOutflg = false;
		frcnt = 0;
	}

	frcnt++;
	//次の状態の設定
	
	if (frcnt < (FINISHFRM / 2)) {
		g_blackOutData.BlackOutNextState = FADE_IN;
	}
	else if (frcnt >(FINISHFRM / 2)) {
		g_blackOutData.BlackOutNextState = FADE_OUT;
	}
	else {
		g_blackOutData.BlackOutNextState = BLACKOUT;
	}
	if (g_blackOutData.BlackOutNextState == COMPLETION) {
		g_blackOutData.BlackOutNextState = NON;
	}
	if (frcnt == FINISHFRM) {
		g_blackOutData.BlackOutNextState = COMPLETION;
	}
}