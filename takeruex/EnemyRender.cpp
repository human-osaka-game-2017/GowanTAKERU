#include"EnemyRender.h"
#include"EnemyControl.h"
#include"MainRender.h"
#include"CommonRender.h"
#include"FileManagement.h"
#include"DirectXGraphics.h"

void EnemyRender() {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	Enemy* enemy = GetenemyData();

	CUSTOMVERTEX Enemy[] = {
		{-ENEMYRESIZEWIDHE / 2, -ENEMYRESIZEHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
		{ ENEMYRESIZEWIDHE / 2, -ENEMYRESIZEHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ ENEMYRESIZEWIDHE / 2,  ENEMYRESIZEHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{-ENEMYRESIZEWIDHE / 2,  ENEMYRESIZEHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	if (enemy->beDead == false && enemy->beActive == true)
	{

		CUSTOMVERTEX DrawVertex[4];
		for (int i = 0; i < 4; i++) {
			DrawVertex[i] = Enemy[i];
			DrawVertex[i].x += enemy->WindowPos.x;
			DrawVertex[i].y += enemy->WindowPos.y;
		}

		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[ENEMY01_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, DrawVertex, sizeof(CUSTOMVERTEX));
	}


}