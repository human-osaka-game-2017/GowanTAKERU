#include"BulletRender.h"
#include"BulletControl.h"
#include"DirectXGraphics.h"
#include"FileManagement.h"
#include"MainRender.h"

#define BULLETPNGSIZE 128

void BulletRender() {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	Bullet* pFirstBullet = GetFirstBulletAddress();
	for (Bullet* pSearchBullet = pFirstBullet->next; pSearchBullet != NULL; pSearchBullet = pSearchBullet->next) {
		CUSTOMVERTEX Bullet[] = {
			{ -pSearchBullet->Size / 2,-pSearchBullet->Size / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
			{ pSearchBullet->Size / 2,-pSearchBullet->Size / 2,0.5f,1.0f,0xFFFFFFFF,pSearchBullet->Size/ BULLETPNGSIZE,0.0f },
			{ pSearchBullet->Size / 2, pSearchBullet->Size / 2,0.5f,1.0f,0xFFFFFFFF,pSearchBullet->Size / BULLETPNGSIZE,pSearchBullet->Size / BULLETPNGSIZE },
			{ -pSearchBullet->Size / 2, pSearchBullet->Size / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,pSearchBullet->Size / BULLETPNGSIZE }
		};

		CUSTOMVERTEX DrawVertex[4];
		for (int j = 0; j < 4; j++) {
			DrawVertex[j] = Bullet[j];
			DrawVertex[j].x += pSearchBullet->WindowPos.x;
			DrawVertex[j].y += pSearchBullet->WindowPos.y;

			if (!pSearchBullet->wasReflect) {
				switch (pSearchBullet->BulletKind) {

				case BULLET01:
					DrawVertex[j].tu += pSearchBullet->Size / BULLETPNGSIZE;
					break;

				case HOMING:
					DrawVertex[j].tu += pSearchBullet->Size / BULLETPNGSIZE * 3;
					break;

				case NONREFLECT:
					DrawVertex[j].tu += pSearchBullet->Size / BULLETPNGSIZE * 2;
					break;
				}
			}

		}
			// テクスチャをステージに割り当てる
			pD3Device->SetTexture(0, pTexture[BULLET_TEX]);
			// 描画
			pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, DrawVertex, sizeof(CUSTOMVERTEX));
	}
}