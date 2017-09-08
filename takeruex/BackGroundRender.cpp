#include"BackGroundRender.h"
#include"DirectXGraphics.h"
#include"FileManagement.h"
#include"MainRender.h"
#include"PlayerControl.h"
#include"MapControl.h"

#define BACKGROUNDWIDTH 2560.0f
#define BACKGROUNDHEIGHT 720.0f

void BackgroundRender() {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	CUSTOMVERTEX Backgrond[] = {
		{ 0.0f,0.0f,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ BACKGROUNDWIDTH,0.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ BACKGROUNDWIDTH,BACKGROUNDHEIGHT,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ 0.0f,BACKGROUNDHEIGHT,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	STAGE_ID stage_ID = GetStage_ID();

	int maptipX = GetStageXYMAX(stage_ID, X);

	float mapX = maptipX * (TIPSIZE);

	D3DXVECTOR2* pBasePoint = GetBasePoint();

	float pos = (pBasePoint->x - DISPLAY_WIDTH / 2) / mapX;

	for (int i = 0; i < 4; i++) {
		Backgrond[i].x -= (BACKGROUNDWIDTH - DISPLAY_WIDTH) * pos;
	}

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