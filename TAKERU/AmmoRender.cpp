#include"AmmoRender.h"
#include"Render.h"

void AMMORender() {

	CUSTOMVERTEX ENEMYAMMO01 [] = {
		{ g_enemyAmmo.cx- AmmoSize/2,g_enemyAmmo.cy- AmmoSize/2,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ g_enemyAmmo.cx+ AmmoSize/2,g_enemyAmmo.cy- AmmoSize/2,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ g_enemyAmmo.cx+ AmmoSize/2,g_enemyAmmo.cy+ AmmoSize/2,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ g_enemyAmmo.cx- AmmoSize/2,g_enemyAmmo.cy+ AmmoSize/2,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	// テクスチャをステージに割り当てる
	g_pD3Device->SetTexture(0, g_pTexture[ENEMYAMMO01_TEX]);
	// 描画
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, ENEMYAMMO01, sizeof(CUSTOMVERTEX));

}