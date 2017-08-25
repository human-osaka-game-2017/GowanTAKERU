#include"Boss2Render.h"
#include"FileManagement.h"
#include"MainRender.h"
#include"Boss2Control.h"
#include"DirectXGraphics.h"

void Boss2Render() {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	Boss2Data* pBoss2 = GetBoss2Data();

	CUSTOMVERTEX boss1[] = {
		{ -BOSS2WIDTH / 2, -BOSS2HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
		{ BOSS2WIDTH / 2, -BOSS2HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,BOSS2WIDTH / ENEMYPNGSIZE,0.0f },
		{ BOSS2WIDTH / 2,  BOSS2HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,BOSS2WIDTH / ENEMYPNGSIZE,BOSS2HEIGHT / ENEMYPNGSIZE },
		{ -BOSS2WIDTH / 2,  BOSS2HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,BOSS2HEIGHT / ENEMYPNGSIZE }
	};
	if (pBoss2->isActive && !pBoss2->isDead) {
		CUSTOMVERTEX drawVertex[4];
		for (int j = 0; j < 4; j++) {
			drawVertex[j] = boss1[j];
			drawVertex[j].x += pBoss2->WindowPos.x;
			drawVertex[j].y += pBoss2->WindowPos.y;
		}

		TrimingVertex(drawVertex, 0.0f, 512.0f, BOSS2WIDTH, BOSS2HEIGHT, ENEMYPNGSIZE, ENEMYPNGSIZE);

		if (!pBoss2->isLeft) {
			TurnVertex_tu(drawVertex);
		}

		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[ENEMY_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawVertex, sizeof(CUSTOMVERTEX));
	}
}