#include"AmmoRender.h"
#include"Ammo.h"
#include"Render.h"

void AMMORender() {
	
	for (int i = 0; i < AmmoNumber; i++) {

		if (g_enemyAmmo[i].berender == true) {

			CUSTOMVERTEX ENEMYAMMO01[] = {
				{ g_enemyAmmo[i].cx - AmmoSize / 2,g_enemyAmmo[i].cy - AmmoSize / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
				{ g_enemyAmmo[i].cx + AmmoSize / 2,g_enemyAmmo[i].cy - AmmoSize / 2,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
				{ g_enemyAmmo[i].cx + AmmoSize / 2,g_enemyAmmo[i].cy + AmmoSize / 2,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
				{ g_enemyAmmo[i].cx - AmmoSize / 2,g_enemyAmmo[i].cy + AmmoSize / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
			};

			// テクスチャをステージに割り当てる
			g_pD3Device->SetTexture(0, g_pTexture[ENEMYAMMO01_TEX]);
			// 描画
			g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, ENEMYAMMO01, sizeof(CUSTOMVERTEX));
		}
	}
}