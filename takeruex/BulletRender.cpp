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
	EditableBulletData* bulletInitializeData = GetEditableBulletData();
	for (Bullet* pSearchBullet = pFirstBullet->next; pSearchBullet != NULL; pSearchBullet = pSearchBullet->next) {
		CUSTOMVERTEX Bullet[] = {
			{ -bulletInitializeData[pSearchBullet->BulletKind].Size / 2,-bulletInitializeData[pSearchBullet->BulletKind].Size / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
			{ bulletInitializeData[pSearchBullet->BulletKind].Size / 2,-bulletInitializeData[pSearchBullet->BulletKind].Size / 2,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
			{ bulletInitializeData[pSearchBullet->BulletKind].Size / 2, bulletInitializeData[pSearchBullet->BulletKind].Size / 2,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
			{ -bulletInitializeData[pSearchBullet->BulletKind].Size / 2, bulletInitializeData[pSearchBullet->BulletKind].Size / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
		};

		CUSTOMVERTEX DrawVertex[4];
		for (int j = 0; j < 4; j++) {
			DrawVertex[j] = Bullet[j];
			DrawVertex[j].x += pSearchBullet->WindowPos.x;
			DrawVertex[j].y += pSearchBullet->WindowPos.y;
		}
		if (pSearchBullet->wasReflect) {
			TrimingVertex(DrawVertex, 0.0f, 0.0f, bulletInitializeData[pSearchBullet->BulletKind].Size, bulletInitializeData[pSearchBullet->BulletKind].Size, BULLETPNGSIZE, BULLETPNGSIZE);
		}
		else {
			TrimingVertex(DrawVertex, bulletInitializeData[pSearchBullet->BulletKind].tu, bulletInitializeData[pSearchBullet->BulletKind].tv, bulletInitializeData[pSearchBullet->BulletKind].Size, bulletInitializeData[pSearchBullet->BulletKind].Size, BULLETPNGSIZE, BULLETPNGSIZE);
		}

		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[BULLET_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, DrawVertex, sizeof(CUSTOMVERTEX));

	}
}