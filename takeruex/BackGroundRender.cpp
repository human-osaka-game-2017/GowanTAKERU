#include"BackGroundRender.h"
#include"DirectXGraphics.h"
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

	STAGE_ID stage_ID = GetStage_ID();
	switch (stage_ID) {
	case STAGE_1:
		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[STAGE_1_BACKGROUND_TEX]);
		break;

	case STAGE_2:
		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[STAGE_2_BACKGROUND_TEX]);
		break;

	case STAGE_3:
		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[STAGE_3_BACKGROUND_TEX]);
		break;

	case STAGE_4:
		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[STAGE_4_BACKGROUND_TEX]);
		break;

	case STAGE_5:
		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[STAGE_5_BACKGROUND_TEX]);
		break;
	}
		
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Backgrond, sizeof(CUSTOMVERTEX));
}