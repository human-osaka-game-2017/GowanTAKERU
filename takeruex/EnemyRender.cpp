#include"EnemyRender.h"
#include"EnemyControl.h"
#include"MainRender.h"
#include"FileManagement.h"
#include"DirectXGraphics.h"
#include "MapRender.h"

void EnemyRender() {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	Enemy* enemy = GetEnemyData();

	int enemyMax = GetEnemyMax();
	for (int i = 0; i < enemyMax; i++) {

		if (enemy[i].beDead == false && enemy[i].beActive == true)
		{
			CUSTOMVERTEX Enemy[] = {
				{ -enemy[i].Width / 2, -enemy[i].Height / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
				{ enemy[i].Width / 2, -enemy[i].Height / 2, 0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
				{ enemy[i].Width / 2,  enemy[i].Height / 2, 0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
				{ -enemy[i].Width / 2,  enemy[i].Height / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
			};

			CUSTOMVERTEX DrawVertex[4];
			for (int j = 0; j < 4; j++) {
				DrawVertex[j] = Enemy[j];
				DrawVertex[j].x += enemy[i].WindowPos.x;
				DrawVertex[j].y += enemy[i].WindowPos.y;

			}

				TrimingVertex(DrawVertex, enemy[i].tu, enemy[i].tv, enemy[i].Width, enemy[i].Height, ENEMYPNGSIZE, ENEMYPNGSIZE);

				if (enemy[i].enemyKind == FIXEDBATTERY1 || enemy[i].enemyKind == FIXEDBATTERY2) {
					if ((enemy[i].bulletFrameCount % 12) < 6) {
						TrimingVertex(DrawVertex, 384.0f, 320.0f, enemy[i].Width, enemy[i].Height, ENEMYPNGSIZE, ENEMYPNGSIZE);
					}
				}

			if (!enemy[i].beLeft) {
				TurnVertex_tu(DrawVertex);
			}

			// テクスチャをステージに割り当てる
			pD3Device->SetTexture(0, pTexture[ENEMY_TEX]);
			// 描画
			pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, DrawVertex, sizeof(CUSTOMVERTEX));

		}
	}
}