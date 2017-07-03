#include"BulletRender.h"
#include"BulletControl.h"
#include"DirectXGraphics.h"
#include"FileManagement.h"
#include"MainRender.h"

void BulletRender() {

	Bullet* bullet = GetBullet();

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	for (int i = 0; i < BULLETNUMBER; i++) {

		if (bullet[i].beActive) {

			CUSTOMVERTEX Bullet[] = {
				{-(bullet + i)->Size / 2,-(bullet + i)->Size / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
				{ (bullet + i)->Size / 2,-(bullet + i)->Size / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
				{ (bullet + i)->Size / 2, (bullet + i)->Size / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
				{-(bullet + i)->Size / 2, (bullet + i)->Size / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f }
			};

			CUSTOMVERTEX DrawVertex[4];
			for (int j = 0; j < 4; j++) {
				DrawVertex[j] = Bullet[j];
				DrawVertex[j].x += bullet[i].WindowPos.x;
				DrawVertex[j].y += bullet[i].WindowPos.y;
			}

			// テクスチャをステージに割り当てる
			pD3Device->SetTexture(0, pTexture[BULLET01_TEX]);
			// 描画
			pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, DrawVertex, sizeof(CUSTOMVERTEX));

		}
	}
}