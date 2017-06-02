#include"Render.h"
#include"BackgroundRender.h"

void BackgroundRender() {

	CUSTOMVERTEX Backgrond[] = {
		{ 0.0f,0.0f,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ DISPLAY_WIDTH,0.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ DISPLAY_WIDTH,DISPLAY_HEIGHT,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ 0.0f,DISPLAY_HEIGHT,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	// テクスチャをステージに割り当てる
	g_pD3Device->SetTexture(0, g_pTexture[BACKGROUND_TEX]);
	// 描画
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Backgrond, sizeof(CUSTOMVERTEX));

}