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

			switch (enemy[i].enemyKind) {
			case WALKINGENEMY_1:
			case WALKINGENEMY_2:
			case WALKINGENEMY_3:
			case WALKINGENEMY_5:
			case WALKINGENEMY_HAS_KEY_1:
			case WALKINGENEMY_HAS_KEY_2:
			case WALKINGENEMY_HAS_KEY_3:
				TrimingVertex(DrawVertex, 0.0f, 0.0f, enemy[i].Width, enemy[i].Height, ENEMYPNGSIZE, ENEMYPNGSIZE);
				break;

			case WALKINGENEMY_4:
				TrimingVertex(DrawVertex, 256.0f, 0.0f, enemy[i].Width, enemy[i].Height, ENEMYPNGSIZE, ENEMYPNGSIZE);
				break;

			case WALKINGENEMY_6:
			case WALKINGENEMY_HAS_KEY_4:
			case WALKINGENEMY_HAS_KEY_5:
				TrimingVertex(DrawVertex, 128.0f, 0.0f, enemy[i].Width, enemy[i].Height, ENEMYPNGSIZE, ENEMYPNGSIZE);
				break;

			case FLYINGENEMY1://空中敵
			case FLYINGENEMY2:
			case FLYINGENEMY3:
			case FLYINGENEMY5:
			case FLYINGENEMY_HAS_KEY1:
			case FLYINGENEMY_HAS_KEY2:
			case FLYINGENEMY_HAS_KEY3:
				TrimingVertex(DrawVertex, 0.0f, 160.0f, enemy[i].Width, enemy[i].Height, ENEMYPNGSIZE, ENEMYPNGSIZE);
				break;
			case FLYINGENEMY4:
				TrimingVertex(DrawVertex, 128.0f, 160.0f, enemy[i].Width, enemy[i].Height, ENEMYPNGSIZE, ENEMYPNGSIZE);
				break;

			case FIXEDBATTERY1://固定砲台
			case FIXEDBATTERY2:
				TrimingVertex(DrawVertex, 0, 320.0f, enemy[i].Width, enemy[i].Height, ENEMYPNGSIZE, ENEMYPNGSIZE);
				break;

			case SWITCH_1:
			case SWITCH_2:
			case SWITCH_3:
				if (enemy[i].Hp == SEITCHMAXHP) {
					TrimingVertex(DrawVertex, 128.0f, 320.0f, enemy[i].Width, enemy[i].Height, ENEMYPNGSIZE, ENEMYPNGSIZE);
				}
				else {
					TrimingVertex(DrawVertex, 256.0f, 320.0f, enemy[i].Width, enemy[i].Height, ENEMYPNGSIZE, ENEMYPNGSIZE);
				}
				break;
			
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