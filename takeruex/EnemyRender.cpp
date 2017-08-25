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

	CUSTOMVERTEX Enemy[] = {
		{-ENEMYRESIZEWIDTH / 2, -ENEMYRESIZEHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
		{ ENEMYRESIZEWIDTH / 2, -ENEMYRESIZEHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ ENEMYRESIZEWIDTH / 2,  ENEMYRESIZEHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{-ENEMYRESIZEWIDTH / 2,  ENEMYRESIZEHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	int enemyMax = GetEnemyMax();
	for (int i = 0; i < enemyMax; i++) {
		if (enemy[i].beDead == false && enemy[i].beActive == true)
		{
			CUSTOMVERTEX DrawVertex[4];
			for (int j = 0; j < 4; j++) {
				DrawVertex[j] = Enemy[j];
				DrawVertex[j].x += enemy[i].WindowPos.x;
				DrawVertex[j].y += enemy[i].WindowPos.y;

			}

			switch (enemy[i].enemyKind) {
			case WALKINGENEMY_1:
				break;

			case WALKINGENEMY_2:
				break;

			case WALKINGENEMY_3:
				break;

			case WALKINGENEMY_4:
				break;

			case WALKINGENEMY_5:
				break;

			case WALKINGENEMY_HAS_KEY_1:
				break;

			case WALKINGENEMY_HAS_KEY_2:
				break;

			case WALKINGENEMY_HAS_KEY_3:
				break;

			case FIXEDBATTERY1://ŒÅ’è–C‘ä
				break;

			case DUMMY://—\”õ
				break;

			case FLYINGENEMY1://‹ó’†“G
				break;

			case FLYINGENEMY2:
				break;

			case FLYINGENEMY3:
				break;

			case FLYINGENEMY4:
				break;

			case FLYINGENEMY5:
				break;

			case FLYINGENEMY_HAS_KEY1:
				break;

			case FLYINGENEMY_HAS_KEY2:
				break;

			case FLYINGENEMY_HAS_KEY3:
				break;

			case FIXEDBATTERY2:
				break;
			}

			// ƒeƒNƒXƒ`ƒƒ‚ðƒXƒe[ƒW‚ÉŠ„‚è“–‚Ä‚é
			pD3Device->SetTexture(0, pTexture[ENEMY_TEX]);
			// •`‰æ
			pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, DrawVertex, sizeof(CUSTOMVERTEX));

		}
	}
}