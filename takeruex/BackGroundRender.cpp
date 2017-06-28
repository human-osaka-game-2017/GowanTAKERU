#include"BackGroundRender.h"
#include"DirectXGraphics.h"
#include"CommonRender.h"
#include"FileManagement.h"
#include"MainRender.h"
#include"PlayerControl.h"

void BackgroundRender() {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	CUSTOMVERTEX Backgrond[] = {
		{ 0.0f,0.0f,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ DISPLAY_WIDTH,0.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ DISPLAY_WIDTH,DISPLAY_HEIGHT,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ 0.0f,DISPLAY_HEIGHT,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	Player* player = GetplayerData();
	if (player->beActive) {
		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[BACKGROUND_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Backgrond, sizeof(CUSTOMVERTEX));
	}
	else {
		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[GAMEOVER_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Backgrond, sizeof(CUSTOMVERTEX));
	}
}